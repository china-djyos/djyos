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

//����ģ��: gui
//����:  ������.
//�汾��V1.0.0
//�ļ�����: gui kernel���岿��
//����˵��:
//�޶���ʷ:
//2. ����: 2011-07-18
//   ����:  ������.
//   �°汾��: V1.1.0
//   �޸�˵��: �������ݽṹʵ���Ѿ���Ϊ���ϲ��ṩ�������ļ�һֱû�ģ��޸�֮��
//             ����__gk_vmalloc��4��������ʹ���Դ����������Ҫ��ʱ��driver���Բ�
//             �ṩ�����ڴ�ĺ�����
//1. ����: 2009-11-21
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "lock.h"
#include "systime.h"
#include "gkernel.h"
#include "align.h"
#include "pool.h"
#include "msgqueue.h"
#include "djyos.h"
#include "gk_usercall.h"
#include <gui/gk_display.h>
#include "gk_win.h"
#include "gk_draw.h"
#include "gk_clip.h"
#include "charset.h"
#include "font.h"
#include "math.h"
#include "string.h"
#include "object.h"
#include "loc_string.h"
//#include "gkernel_config.h"
#include "component_config_gk.h"
#include "dbug.h"

struct GkWinObj   *g_ptFocusWin;//��ʾ���㴰��,���е���ʾ���Ƕ�������ڵĲ���
static struct Object *s_ptDisplayDir;
static struct Object *s_ptWindowDir;
u32 g_u32DefaultBackColor,g_u32DefaultColor;
u16 g_u16GkServerEvtt,g_u16GkUsercallServerEvtt;
u16 g_u16GkServerEvent,g_u16GkUsercallServerEvent;


struct GkChunnel g_tGkChunnel;

extern struct MemCellPool *g_ptClipRectPool; //from gk_clip.c

//����������
#define CN_CLIP_INIT_NUM    100
struct ClipRect g_tClipRect[CN_CLIP_INIT_NUM];
struct GkWinObj *g_ptZ_Topmost;

//struct SemaphoreLCB *g_ptUsercallSemp;
//������÷�ϣ��gui kernel��������ٷ��أ�ʹ������ź���
struct SemaphoreLCB *g_ptSyscallSemp;
//struct MutexLCB *g_ptGkServerSync;

u32 __ExecOneCommand(u16 DrawCommand,u8 *ParaAddr);

//gkwin����Դ���ֵ�λ����z�����ϵ:
// 1������Դ���У������Ӵ��ڰ�z�����У���ǰ�˵��Ǹ�����ָ��ĵ�һ����㡣
// 2��z_prio��ԱӰ��win�����ֵܽ���е�λ�ã�ԽСԽǰ��
// 3��z_prioһ����Ϊ0��<=0���ڸǸ����ڣ���>0�򱻸������ڸ�

static u8 draw_chunnel_buf[CFG_GKERNEL_CMD_DEEP];   //�ѻ��λ������е�����һ��ȫ������
static u8 write_chunnel_buf[CFG_GKERNEL_CMD_DEEP];  //���ں��ϲ㽻���Ļ��λ�����
//----��ʼ��guiģ��---------------------------------------------------------
//����: ��ʼ��guiģ�飬����Դ���������ӱ�Ҫ����Դ���
//����: ģ���ʼ��������û�в���
//����: true = �ɹ�
//-----------------------------------------------------------------------------
bool_t ModuleInstall_GK(void)
{
    s_ptDisplayDir = OBJ_NewChild(OBJ_GetRoot(), (fnObjOps)-1, 0, "display");
    s_ptWindowDir = OBJ_NewChild(OBJ_GetRoot(), (fnObjOps)-1, 0, "gkwindow");

    Ring_Init(&g_tGkChunnel.ring_syscall,(u8 *)write_chunnel_buf,CFG_GKERNEL_CMD_DEEP);

    g_tGkChunnel.syscall_mutex = Lock_MutexCreate("gui chunnel to gk mutex");
//  g_tGkChunnel.syscall_semp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"gui chunnel to gk semp");
    g_tGkChunnel.usercall_semp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"gui chunnel from gk semp");

    g_tGkChunnel.usercall_msgq = MsgQ_Create(CN_USERCALL_MSGQ_SIZE,CN_USERCALL_MSG_SIZE,0);

//    g_ptGkServerSync = Lock_MutexCreate("gk server sync");
//  g_ptGkServerSync = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"gk server sync");
//  g_ptUsercallSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"gk wait repaint");
    g_ptSyscallSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"gk wait job");
    g_u16GkServerEvtt = DJY_EvttRegist(EN_CORRELATIVE,249,0,0,__GK_Server,
                                    NULL,0x1000,"gui kernel server");

//  g_u16GkUsercallServerEvtt= DJY_EvttRegist(EN_CORRELATIVE,249,0,0,
//                  __GK_UsercallServer,NULL,4096,"gkernel usercall server");

    g_ptClipRectPool = Mb_CreatePool(&g_tClipRect,
                                  CN_CLIP_INIT_NUM,
                                  sizeof(struct ClipRect),
                                  100,2000, "clip area");

    g_u16GkServerEvent = DJY_EventPop(g_u16GkServerEvtt,NULL,0,0,0,0);
//  g_u16GkUsercallServerEvent = DJY_EventPop(g_u16GkUsercallServerEvtt,NULL,0,0,0,0);

    if(    (g_ptClipRectPool == NULL)
        || (g_u16GkServerEvent == CN_EVENT_ID_INVALID)
        || (g_u16GkUsercallServerEvent == CN_EVENT_ID_INVALID)
        || (g_ptSyscallSemp == NULL)
//      || (g_ptUsercallSemp == NULL)
//        || (g_ptGkServerSync == NULL)
        || (g_tGkChunnel.usercall_semp == NULL)
//      || (g_tGkChunnel.syscall_semp == NULL)
        || (g_tGkChunnel.syscall_mutex == NULL)
        || (g_tGkChunnel.usercall_msgq == NULL) )
    {
        goto exit_error;
    }

    return true;

exit_error:
    Mb_DeletePool(g_ptClipRectPool);
    DJY_EvttUnregist(g_u16GkServerEvtt);
    DJY_EvttUnregist(g_u16GkUsercallServerEvtt);
    Lock_SempDelete(g_ptSyscallSemp);
//  Lock_SempDelete(g_ptUsercallSemp);
//  Lock_SempDelete(g_ptGkServerSync);
//    Lock_MutexDelete(g_ptGkServerSync);
    Lock_SempDelete(g_tGkChunnel.usercall_semp);
//  Lock_SempDelete(g_tGkChunnel.syscall_semp);
    Lock_MutexDelete(g_tGkChunnel.syscall_mutex);
    MsgQ_Delete(g_tGkChunnel.usercall_msgq);

    OBJ_Delete(s_ptDisplayDir);
    OBJ_Delete(s_ptWindowDir);
    return false;
}

//�������ظ�ʽ��������ʾ��������Ҫ���ڴ�ߴ硣
u32 __gk_cal_buf_size(u32 xsize,u32 ysize,u32 *linesize,u16 PixelFormat)
{
    u32 size;
    u8 color_bits;
    color_bits = (PixelFormat&CN_PF_BITWIDE_MSK)>>8;
    switch(color_bits)
    {
        case 1:
        {
            *linesize = (xsize + 7)/8;
            size = *linesize*ysize;
        }break;
        case 2:
        {
            *linesize = (xsize + 3)/4;
            size = *linesize*ysize;
        }break;
        case 4:
        {
            *linesize = (xsize + 1)/2;
            size = *linesize*ysize;
        }break;
        case 8:
        {
            *linesize = xsize;
            size = xsize * ysize;
        }break;
        case 16:
        {
            *linesize = 2*xsize;
            size = xsize*ysize*2;
        }break;
        case 24:
        {
            *linesize = 3*xsize;
            size = xsize*ysize*3;
        }break;
        case 32:
        {
            *linesize = 4*xsize;
            size = xsize*ysize*4;
        }break;
        default:
            size = 0;
            *linesize = 0;
            break;
    }
    return size;

}

//----���䴰�ڻ���-------------------------------------------------------------
//����: ���䴰�ڻ��棬������1�����������bitmap,2������ô�����rop4���ԣ���Ҫ��
//      ��bitmsk_buf��3��pattern buffer��
//      ��Ȼ������������Ҫ���������֣���ʵ�����Ǻϲ�һ��Ӷ������룬Ȼ�����ֹ�
//      �ָ�ɼ����������ġ��������������byte��ָ�룬�����뵣�Ķ������⡣
//����: buf�����ڷ��ؽ���Ľṹָ��
//      xsize��ysize����ʾ����ߴ�
//      rop_code�����ڵ�rop_code���룬Ӱ���Ƿ����buf�е�rop4_msk��Ա��
//����: true=�ɹ���false=ʧ��
//ע: 2416��2D���ٲ�֧��rop4�������ж�rop_code��ֻ�����pat��vbuf����
//-----------------------------------------------------------------------------
bool_t __gk_vmalloc(struct DisplayObj *disp,struct GkWinObj *gkwin,
                    u16 PixelFormat)
{
    u8 *mem;
    u32 buf_size,linesize;// pal_size=0;
    s32 xsize,ysize;
    bool_t result;
    xsize = gkwin->right - gkwin->left;
    ysize = gkwin->bottom - gkwin->top;
    if(PixelFormat == CN_SYS_PF_DISPLAY)
        PixelFormat = disp->pixel_format;
    //������ʾ��������Ҫ���ڴ�ߴ硣
    buf_size = __gk_cal_buf_size(xsize,ysize,&linesize,PixelFormat);
    buf_size = align_up_sys(buf_size);
    //ͬʱ����bitmap���ݽṹ��ͼƬλͼ
    mem = (u8*)M_MallocLcHeap(buf_size + sizeof(struct RectBitmap),
                            disp->DisplayHeap,0);
    if(mem != NULL)
    {
        memset(mem, 0, buf_size + sizeof(struct RectBitmap));
        gkwin->wm_bitmap = (struct RectBitmap*)mem;
        gkwin->wm_bitmap->PixelFormat = PixelFormat;
        gkwin->wm_bitmap->width = xsize;
        gkwin->wm_bitmap->height = ysize;
        gkwin->wm_bitmap->linebytes = linesize;
        mem += sizeof(struct RectBitmap);
        gkwin->wm_bitmap->bm_bits = mem;
        result = true;
    }else
    {
        gkwin->wm_bitmap = NULL;
        result = false;
    }
    return result;
}


//----���·��䴰�ڻ���---------------------------------------------------------
//����: �����ڳߴ�ı䣬���ñ��������·��仺�档�߼��ϣ��������Ĺ���="vfree-
//      ����vmalloc"����ϣ�������һ����������Ϊ�����Ч�ʡ���Ϊ�ڴ��ǰ������
//      �ģ��ڴ�������൱��ʱ�Ĺ�����vmalloc����Ĵ��ڻ��棬���ܱ�ʵ�ʵĴ󣬵�
//      ���ڳߴ��������Ļ��������������·��䡣���û�ͨ���϶����ı䴰�ڳߴ�ʱ��
//      ʵ������һ��һ��ı�ģ�ÿ�ζ����·���Ļ���ϵͳ�����ܴ����ײ�����Ƭ��
//      vrmalloc�����ȼ��buf�еĻ������Ƿ���Ȼ�������������㣬�����·��䡣
//      �����ܻ����xsize��ysize����ָ�롣
//����: buf��ԭ���ڻ���ָ�룬Ҳ���ڷ��ؽ��
//      xsize��ysize����ʾ����ߴ�
//      rop_code�����ڵ�rop_code����
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t __gk_vrmalloc(struct DisplayObj *disp,struct GkWinObj *gkwin)
{
    u8 *mem;
    u32 buf_size,linesize,oldsize,newsize;//pal_size=0;
    s32 xsize,ysize;
    bool_t remalloc = true,result;
    u16 PixelFormat;
    xsize = gkwin->right - gkwin->left;
    ysize = gkwin->bottom - gkwin->top;
    PixelFormat = gkwin->wm_bitmap->PixelFormat;

    //���·����Դ棬��ԭ�Դ���Ȼ����Ҫ�󣬿��ܲ������·��䡣
    //�������·��䣬�����ȡ�ȷ�����ͷŵİ취����Ϊdjyos�Ƕ��̻߳��������ͷź�
    //����Ļ�����Ȼ���������ӷ���ĳɹ��ʣ����ͷź��������ռ���ڴ汻��ĸ�����
    //���¼���ȥ�Ļ����ͻ���ɷ���ʧ�ܡ��ȷ�����ͷţ��������ʧ�ܣ����ٻ�����
    //ȷ��ԭ�����Դ治��ʧ��
    if(PixelFormat == CN_SYS_PF_DISPLAY)
        PixelFormat = disp->pixel_format;
    //������ʾ��������Ҫ���ڴ�ߴ硣
    buf_size = __gk_cal_buf_size(xsize,ysize,&linesize,PixelFormat);
    buf_size = align_up_sys(buf_size);
    newsize = buf_size + sizeof(struct RectBitmap);

    oldsize = M_CheckSize(gkwin->wm_bitmap); //��ѯԭָ��ʵ�ʷ�����ڴ�
    if(newsize <= oldsize)
    {
        //�µ��Դ�������ԭʵ�ʳߴ������1/8���ڵĻ���С����С�ڴ����ߴ�ģ�
        //�����·����Դ�
        if((oldsize - newsize) <= (oldsize>>3))
            remalloc = false;       //�������·���
    }
    if(remalloc)
    {
        mem =(u8*) M_MallocLcHeap(newsize,disp->DisplayHeap,0);
        if(mem != NULL)
        {
            memset(mem, 0, newsize);
            gkwin->wm_bitmap = (struct RectBitmap*)mem;
            gkwin->wm_bitmap->width = xsize;
            gkwin->wm_bitmap->height = ysize;
            gkwin->wm_bitmap->linebytes = linesize;
            mem += sizeof(struct RectBitmap);
            gkwin->wm_bitmap->bm_bits = mem;

            result = true;
        }else
        {
            result = false;   //�·����ڴ�ʧ�ܣ�����ԭ�����䣬���ͷ�ԭ�ڴ档
        }
    }else                     //�������·���
    {
        result = true;
    }
    return result;
}


//----�ͷŴ��ڻ���-------------------------------------------------------------
//����: �ͷ�һ�����ڵĻ��档
//����: disp������������ʾ��
//      buf�����ڷ��ؽ���Ľṹָ��
//����: ��
//-----------------------------------------------------------------------------
void __gk_vfree(struct DisplayObj *disp,struct GkWinObj *gkwin)
{
    //�����ڴ�ʱ���Դ��bitmap�ṹ��ͬʱ����ģ���ֻ�����
    //m_freeһ�Σ����߾��ͷ�
    M_FreeHeap(gkwin->wm_bitmap,disp->DisplayHeap);
}

//----ȡz��Ƭ�ε���ʼ����(���ڼ������ﴰ��)------------------------------------
//����: ÿһ�����ڼ������ﴰ�ڣ�����z����ռ��������һ�Σ�������ȡ�öε���ʼ����(
//      ���˵Ĵ���)��
//����: gkwin������ָ��
//����: ��ʼ����ָ��
//-----------------------------------------------------------------------------
struct GkWinObj *__GK_GetZsectionStart(struct GkWinObj *gkwin)
{
    struct Object *temp;
    struct GkWinObj *current,*result;
    temp = gkwin->HostObj;
    result = gkwin;
    while(1)
    {
        temp = OBJ_GetChild(temp);
        if(temp == NULL)        //result�Ѿ�û���Ӵ�����
            break;
        temp = OBJ_GetPrev(temp);
        current = (struct GkWinObj*)OBJ_GetPrivate(temp);
        if(current->WinProperty.Zprio <= CN_ZPRIO_DEFAULT )
            break;      //previous�������Ӵ��������˵Ĵ��ڣ���resultǰ�ˣ�����
                        //�����Ӵ��ھ���resultǰ��
        else
        {
            result = current;
        }
    }
    return result;
}

//----ȡz��Ƭ�εĽ�������(���ڼ����Ӵ���)--------------------------------------
//����: ÿһ�����ڼ������ﴰ�ڣ�����z����ռ��������һ�Σ�������ȡ�öεĽ�������(
//      ��ǰ�˵Ĵ���)��
//����: gkwin������ָ��
//����: ��������ָ��
//-----------------------------------------------------------------------------
struct GkWinObj *__GK_GetZsectionEnd(struct GkWinObj *gkwin)
{
    struct Object *temp;
    struct GkWinObj *current,*result;
    temp = gkwin->HostObj;
    result = gkwin;
    while(1)
    {
        temp = OBJ_GetChild(temp);
        if(temp == NULL)        //result�Ѿ�û���Ӵ�����
            break;
        current = (struct GkWinObj*)OBJ_GetPrivate(temp);
        if(current->WinProperty.Zprio > CN_ZPRIO_DEFAULT )
            break;      //��һ���Ӵ����������Ӵ�������ǰ�˵Ĵ��ڣ�����result���
                        //���������Ӵ��ھ���result���
        else
        {
            result = current;
        }
    }
    return result;
}


//----��������-----------------------------------------------------------------
//����: ������������ʾ���ĵ�һ�����ڣ����е���ʾ���������ȴ���������ܴ������ڣ�
//      ����ĳߴ粻��С����ʾ���ĳߴ磬�����Դ�����ʾ���ߴ硣������ˢ��screen
//      �Ĺ����к���ͨ�����ǵ�ͬ�ġ�
//����: para���������ݽṹ�����Ա������ṹ���崦��
//����: �´��ھ��(ָ��)
//-----------------------------------------------------------------------------
struct GkWinObj *__GK_CreateDesktop(struct GkscParaCreateDesktop *para)
{
    struct GkWinObj *desktop;
    struct DisplayObj *display;
    struct Object *NewWindow;
    struct ClipRect *clip;
    bool_t BufM;
    s32 desktop_x,desktop_y;
    u32 msk_size;
    struct GkscParaFillWin para_fill;
    if(para->display == NULL)
    {
        return NULL;
    }
    display = para->display;
    if(para->width >= display->width)
        desktop_x = para->width;            //������Դ�����ʾ���ߴ�
    else
        desktop_x = display->width;   //���治��С����ʾ���ߴ�
    if(para->height >= display->height)
        desktop_y = para->height;
    else
        desktop_y = display->height;
    if(display->frame_buffer == NULL)
    {
        if(para->buf_mode == CN_WINBUF_BUF)
        {
            return NULL;        //��֡�������ʾ��������������������
        }
        BufM = false;
    }
    else
    {
        if(para->buf_mode == CN_WINBUF_NONE)
            BufM = false;
        else
            BufM = true;
    }
    desktop = para->desktop;    //para->desktop�ɵ������ṩ�ڴ棬��ָ�����
    //�����ʼ�����棬����Ҳ�ǰ����ڹ����
    desktop->z_back = desktop;
    desktop->z_top = desktop;
    strncpy(desktop->win_name,para->name,CN_GKWIN_NAME_LIMIT);
    desktop->win_name[CN_GKWIN_NAME_LIMIT] = '\0';
    desktop->redraw_clip = NULL;
    desktop->changed_clip = NULL;
    desktop->disp = display;            //������ʾ��
    desktop->WinProperty.Zprio = CN_ZPRIO_DEFAULT;                //��������ȼ�
    desktop->WinProperty.DestBlend = CN_GKWIN_DEST_HIDE;        //��������������Ч
    desktop->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_NONE; //���������ʾ
    desktop->WinProperty.DirectDraw = CN_GKWIN_UNDIRECT_DRAW; //��ֱ��д��
    desktop->WinProperty.BoundLimit = CN_BOUND_LIMIT;
    desktop->WinProperty.Visible = CN_GKWIN_VISIBLE;
    desktop->RopCode = (struct RopGroup){ 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  }; //����̶�
    desktop->absx0 = 0;
    desktop->absy0 = 0;
    desktop->left = 0;
    desktop->top = 0;
    desktop->right = desktop_x;
    desktop->bottom = desktop_y;
    desktop->limit_left = 0;
    desktop->limit_top = 0;
    desktop->limit_right = display->width;
    desktop->limit_bottom = display->height;

    display->z_topmost = desktop;
    display->desktop = desktop;
    clip = (struct ClipRect*)Mb_Malloc(g_ptClipRectPool,0);
    NewWindow = OBJ_NewChild(s_ptWindowDir, (fnObjOps)-1, (ptu32_t)desktop, (const char*)(desktop->win_name));
    if((clip == NULL) || (NewWindow == NULL))
    {
        Mb_Free(g_ptClipRectPool, clip);
        OBJ_Delete(NewWindow);
        return NULL;
    }
    desktop->HostObj = NewWindow;
    if(BufM)      //����ģʽ��Ϊ����׼��������
    {
        if( __gk_vmalloc(display,desktop,para->PixelFormat) )//�����Դ�
        {
            msk_size = ((desktop_x +7)/8+7)/8*((desktop_y +7)/8);
            //����frame bufferʱ����Ҫchanged_msk
            desktop->changed_msk.bm_bits = M_MallocHeap(msk_size,
                                            display->DisplayHeap,0);
            if( desktop->changed_msk.bm_bits == NULL)   //����changed_msk�ڴ�ʧ��
            {
                display->z_topmost = NULL;
                display->desktop = NULL;
                M_FreeHeap(desktop->changed_msk.bm_bits,display->DisplayHeap);
                Mb_Free(g_ptClipRectPool,clip);
                OBJ_Delete(NewWindow);
                __gk_vfree(display,desktop);
                DJY_SaveLastError(EN_GK_NO_MEMORY);
                debug_printf("gkwin","�Դ治��\n\r");
                return NULL;
            }
            else
            {//����changed_msk�ڴ�ɹ������Դ��ٷ���
                memset(desktop->changed_msk.bm_bits,0,msk_size);
                desktop->changed_msk.PixelFormat = CN_SYS_PF_GRAY1;
                desktop->changed_msk.height = desktop_y/8;
                desktop->changed_msk.width = desktop_x/8;
                desktop->changed_msk.linebytes = ((desktop_x+7)/8+7)/8;
                desktop->HyalineColor = 0;

                desktop->wm_bitmap->ExColor = para->BaseColor;
            }
        }
        else               //�����Դ�ʧ�ܣ�
        {
            Mb_Free(g_ptClipRectPool,clip);
            OBJ_Delete(NewWindow);
            display->z_topmost = NULL;
            display->desktop = NULL;
            DJY_SaveLastError(EN_GK_NO_MEMORY);
            debug_printf("gkwin","�Դ治��\n\r");
            return NULL;
        }

    }
    else                   //�ǻ���ģʽ������û�л�����
    {
        desktop->wm_bitmap = NULL;
        desktop->changed_msk.bm_bits = NULL; //û����һ����в��������bug
    }
    //��������򣬿������С����ʾ���ߴ��Сһ��
    clip->next = clip;
    clip->previous = clip;
    clip->rect.left = 0;
    clip->rect.top = 0;
    clip->rect.right = display->width;
    clip->rect.bottom = display->height;
    desktop->visible_clip = clip;
    desktop->visible_bak = NULL;
    display->reset_clip = true;

    //�ø�������ɫ�������
    para_fill.gkwin = desktop;
    para_fill.color = para->color;
    __GK_FillWin(&para_fill);
    //�����´���ָ��
    return desktop;
}


//----��������-----------------------------------------------------------------
//����: ����һ�����ڣ��´��ڵ����ȼ�Ϊ0��λ���������ȼ�=0�Ĵ��ڵ���ǰ�ˡ�
//����: para���������ݽṹ�����Ա������ṹ���崦��
//����: �´��ھ��(ָ��)
//-----------------------------------------------------------------------------
struct GkWinObj *__GK_CreateWin(struct GkscParaCreateGkwin *para)
{
    u32 size;
    struct RopGroup RopCode;
    s32 width,height;
    bool_t BufM;
    struct GkWinObj *target_section;
    struct GkWinObj *parent;                       //gkwin�ĸ�����
    struct Object *NewWindow;
    struct DisplayObj *display;
    //z���б��ƶ��Ĵ��ڶ����˵Ĵ���
    struct GkWinObj *move_start;
    //z���б��ƶ��Ĵ��ڶ���ǰ�˵Ĵ���
    struct GkWinObj *move_end;
    struct GkWinObj *gkwin;
    struct GkscParaFillWin para_fill;

    width = para->right - para->left;
    height = para->bottom - para->top;
    display = para->parent_gkwin->disp;
//  gkwin = para->gkwin;    //para->gkwin�ɵ������ṩ�ڴ棬��ָ�����
    gkwin = M_MallocLcHeap(sizeof(struct GkWinObj),  display->DisplayHeap, 0);
    if(gkwin == NULL)
        return NULL;
    memset(gkwin, 0, sizeof(struct GkWinObj));
    parent = para->parent_gkwin;
    if((parent == NULL)||(width < 0) || (height < 0))
    {
        return NULL;
    }
    if(parent->wm_bitmap == NULL)
    {
        if(para->buf_mode == CN_WINBUF_BUF)
        {
            return NULL;        //�������޻��壬�������Ӵ����л��塣
        }
        BufM = false;
    }
    else
    {
        if(para->buf_mode == CN_WINBUF_NONE)
            BufM = false;
        else
            BufM = true;
    }

    RopCode = para->RopCode;
//  if((RopCode & (CN_ROP_ROP2_EN + CN_ALPHA_MODE_NEED_AD)) != 0)
//      return NULL;   //�������Բ�֧��rop2����ҪDst Alpha�����alpha���
    if((RopCode.AlphaMode & CN_ALPHA_MODE_NEED_AD) != 0)
        return NULL;   //�������Բ�֧����ҪDst Alpha�����alpha���

    *gkwin = *parent;               //�´��ڿ��������ڵ�����

    strncpy(gkwin->win_name,para->name,CN_GKWIN_NAME_LIMIT);
    gkwin->win_name[CN_GKWIN_NAME_LIMIT] = '\0';

    //�������´��ڵ�����������丸���ڵ�
    gkwin->left = para->left;
    gkwin->top = para->top;
    gkwin->right = para->right;
    gkwin->bottom = para->bottom;
    //���ڵľ���λ�ã����������screen������ԭ��
    gkwin->absx0 = para->left + para->parent_gkwin->absx0;
    gkwin->absy0 = para->top + para->parent_gkwin->absy0;
    gkwin->WinProperty.Zprio = CN_ZPRIO_DEFAULT;                //�½�����Ĭ��Ϊ0
    gkwin->WinProperty.DestBlend = __GK_RopIsNeedDestination(RopCode);
    gkwin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_NONE; //���������ʾ
    gkwin->WinProperty.DirectDraw = CN_GKWIN_UNDIRECT_DRAW; //��ֱ��д��
    gkwin->WinProperty.BoundLimit = CN_BOUND_LIMIT;
    gkwin->WinProperty.Visible = CN_GKWIN_VISIBLE;

    gkwin->RopCode = RopCode;

    if(BufM)              //����ģʽ��Ϊ����׼��������
    {
        if( __gk_vmalloc(display,gkwin,para->PixelFormat) )  //�����Դ�
        {
            size = (((width +7)/8+7)/8 *((height +7)/8));
            //����frame bufferʱ����Ҫchanged_msk
            gkwin->changed_msk.bm_bits = M_MallocHeap(size,
                                                    display->DisplayHeap,0);
            if(gkwin->changed_msk.bm_bits == NULL)   //�����޸������ڴ�ʧ��
            {
                __gk_vfree(display,gkwin);
                DJY_SaveLastError(EN_GK_NO_MEMORY);
                debug_printf("gkwin","�Դ治��\n\r");
                return NULL;
            }
            else
            {
                memset(gkwin->changed_msk.bm_bits,0,size);
                gkwin->changed_msk.PixelFormat = CN_SYS_PF_GRAY1 ;
                gkwin->changed_msk.height = (height + 7)/8;
                gkwin->changed_msk.width = (width + 7)/8;
                gkwin->changed_msk.linebytes = ((width+7)/8+7)/8;
                gkwin->HyalineColor = para->HyalineColor;
                gkwin->wm_bitmap->ExColor = para->BaseColor;
            }
        }
        else               //�����Դ�ʧ�ܣ�
        {
            DJY_SaveLastError(EN_GK_NO_MEMORY);
            debug_printf("gkwin","�Դ治��\n\r");
            return NULL;
        }
    }
    else                   //�ǻ���ģʽ������û�л�����
    {
        gkwin->wm_bitmap = NULL;
        gkwin->changed_msk.bm_bits = NULL;//û����һ����в��������bug
    }

    //���°��´������ӵ���Դ�����У������뵽z����
    if(OBJ_GetChild(para->parent_gkwin->HostObj) == NULL)  //���������Ӵ���
    {
        NewWindow = OBJ_NewChild(parent->HostObj, (fnObjOps)-1, (ptu32_t)gkwin,
                                    (const char*)(gkwin->win_name));
        if(NewWindow == NULL)
            return NULL;
        gkwin->HostObj = NewWindow;
        //����4����z���а�win���븸�ڵ��ǰ��
        gkwin->z_back = parent;
        gkwin->z_top = parent->z_top;
        parent->z_top->z_back = gkwin;
        parent->z_top = gkwin;
        if(display->z_topmost == parent)
            display->z_topmost = gkwin;
    }
    else
    {//���������Ӵ���
        //ȡz���б��ƶ�����gkwinͬ���Ĵ��ڶο�ʼ�ͽ������ڶ�
        move_end = (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetChild(parent->HostObj));
        move_start = (struct GkWinObj*)OBJ_GetPrivate(OBJ_GetPrev(move_end->HostObj));
        target_section = move_end;
        while(1)
        {//ɨ��ͬ�������к�Ŀ�괰�����ȼ���ͬ�Ĵ��ڣ�ɨ����z��Ϊ����
            //ɨ��˳��:��z���ǰ�����˽���ɨ�裬ֻɨ�ֵܴ��ڣ���ɨ�Ӵ���
            if(target_section->WinProperty.Zprio >= CN_ZPRIO_DEFAULT)
                break;
            //ɨ��Ŀ�괰������z������һ���ڣ���ֹ����
            if(target_section == move_start)
                break;
            target_section = (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetNext(target_section->HostObj));
        }
        if(target_section->WinProperty.Zprio == CN_ZPRIO_DEFAULT)
        {//ͬ�������д��ں�gkwin��prio��ȵĴ���
//            NewWindow = OBJ_AddToPrevious(target_section->HostObj, NULL, (ptu32_t)gkwin,
//                                             (const char *)(gkwin->win_name));
            move_end = __GK_GetZsectionEnd(target_section);
            NewWindow = OBJ_NewPrev(target_section->HostObj, (fnObjOps)-1, (ptu32_t)gkwin,
                                     (const char *)(gkwin->win_name));
            if(NewWindow == NULL)
                return NULL;
            gkwin->HostObj = NewWindow;
            if(OBJ_IsHead(target_section->HostObj))
                OBJ_ChildMoveToOrev(parent->HostObj);
            //��ȡtarget_section�����Ӵ�������z��εĽ�������(��ǰ��)
            //�´��ڲ��뵽����ǰ��
            if(move_end == display->z_topmost)
                display->z_topmost = gkwin;
            //�嵽Ŀ�괰��ǰ��

            gkwin->z_back = move_end;
            gkwin->z_top = move_end->z_top;
            move_end->z_top->z_back = gkwin;
            move_end->z_top = gkwin;
        }
        else //�ֵܴ�����û�����ȼ�=0�Ĵ���
        {
            //ͬ�������������ȼ�����0�ģ���Դ�������´��ڲ�����ǰ��
            if(target_section->WinProperty.Zprio > CN_ZPRIO_DEFAULT)
            {
//                NewWindow = OBJ_AddToPrevious(target_section->HostObj, NULL, (ptu32_t)gkwin,
//                                                (const char *)(gkwin->win_name));
                NewWindow = OBJ_NewPrev(target_section->HostObj, (fnObjOps)-1, (ptu32_t)gkwin,
                                         (const char *)(gkwin->win_name));
                if(NewWindow == NULL)
                    return NULL;
            }
            else    //ͬ���������ȼ��������´���(prio = 0)
            {
                //�´�������Դ�����д���ͬ����������
//                NewWindow = OBJ_AddToNext(target_section->HostObj, NULL, (ptu32_t )gkwin,
//                                            (const char*)(gkwin->win_name));
                NewWindow = OBJ_NewNext(target_section->HostObj, (fnObjOps)-1, (ptu32_t )gkwin,
                                            (const char*)(gkwin->win_name));
                if(NewWindow == NULL)
                    return NULL;
            }
            gkwin->HostObj = NewWindow;
            //�´����Ĵ���gkwin���ȼ�Ϊ0������ͬ���������ȼ�����ηֲ�����
            //���ڿ϶��ڸ�����ǰ��
            if(parent == display->z_topmost)
                display->z_topmost = gkwin;
            //�嵽Ŀ�괰��ǰ��
            gkwin->z_back = parent;
            gkwin->z_top = parent->z_top;
            parent->z_top->z_back = gkwin;
            parent->z_top = gkwin;
        }
    }
    //���ô��ڵĿɼ��߽磬���ܴ��ڻ����ڵ�Ӱ�죬����������Դ���к���ܵ���
    __GK_SetBound(gkwin);
    gkwin->visible_bak = NULL;
    gkwin->visible_clip = NULL;
    __GK_ScanNewVisibleClip(display);
    if(!para->unfill)
    {
        para_fill.gkwin = gkwin;
        para_fill.color = para->color;
        __GK_FillWin(&para_fill);
//      gkwin->visible_clip = __GK_FreeClipQueue(gkwin->visible_clip);
    }
    display->reset_clip = true;

    return gkwin;
}

//----�ı䴰�ڳߴ��λ��-------------------------------------------------------
//����: �ı�һ�����ڵĳߴ��Լ�λ�ã����û�ĳߴ磬ʵ���Ͼ���һ��gk_move_win��
//����: gcwin��Ŀ�괰��
//      new_area���µĴ��ڳߴ��λ��
//      mode���ĳߴ�󣬴���ͼ����ģʽ��0=���ֲ��䣬1=�����ѹ����
//            ���崰�ڲ���Ч
//����: true = �ɹ���false = ʧ��(һ�����ڴ���䵼��)
//ע: �л�����������£����ֻ���ƶ���λ�ã�����Ҫ���»��ơ�
//    ����޸��˳ߴ磬Ӧ�ó���Ӧ���ػ棬ֱ���ػ漴�ɣ�����ȴ��ػ���Ϣ��
//-----------------------------------------------------------------------------
bool_t __GK_ChangeWinArea(struct GkscParaChangeWinArea *para)
{
    s32 left,top,right,bottom;
    s32 delta_top,delta_left;
    struct GkWinObj *cwawin,*temp,bak;
    struct Object *changing,*current;
    struct DisplayObj *disp;
    struct GkscParaMoveWin movwin_para;

    cwawin = para->gkwin;
    disp = cwawin->disp;
    left = para->left;
    top = para->top;
    right = para->right;
    bottom = para->bottom;
    if(cwawin == disp->desktop)
    {
        if(((right - left) < disp->width) || ((bottom - top) < disp->height))
            return false;                   //����ߴ粻��С����ʾ���ߴ�
    }
    if((right < left) || (bottom < top))    //�ߴ����ݲ��Ϸ�
        return false;

    if(((right-left) == (cwawin->right-cwawin->left))
            && ((bottom-top) == (cwawin->bottom - cwawin->top)))
    {
        movwin_para.gkwin = cwawin;
        movwin_para.left = left;
        movwin_para.top = top;
        __GK_MoveWin(&movwin_para);        //ֻ���ƶ��˴��ڣ��ߴ�δ��
        return true;
    }
    if(cwawin->wm_bitmap != NULL)
    {
        bak = *cwawin;
        if( !__gk_vrmalloc(disp,&bak))
        {
            DJY_SaveLastError(EN_GK_NO_MEMORY);
            debug_printf("gkwin","�Դ治��\n\r");
            return false;   //�����Դ�ʧ�ܣ����ڳߴ�δ�ı�(��Ȼ��Ч)
        }else
        {
            if(bak.wm_bitmap != cwawin->wm_bitmap)
                M_FreeHeap(cwawin->wm_bitmap,disp->DisplayHeap);
            *cwawin = bak;
        }
    }
    if(cwawin == disp->desktop)
    {   //Ŀ�괰�������棬�ж�������:������븲����ʾ������
        if(left > 0)    //������߽���븲����ʾ��
        {
            cwawin->absx0 = 0;
            delta_left = 0 - cwawin->left;
            cwawin->left = 0;
            cwawin->limit_left = 0;
        }
        else
        {
            cwawin->absx0 = left;
            delta_left = left - cwawin->left;
            cwawin->left = left;
            cwawin->limit_left = -left;//����Ŀ���ʾ��߽�Ӧ�øպ�����ʾ������߽�
        }
        if(top > 0)    //�����ϱ߽���븲����ʾ��
        {
            cwawin->absy0 = 0;
            delta_top = 0 - cwawin->top;
            cwawin->top = 0;
            cwawin->limit_top = 0;
        }
        else
        {
            cwawin->absy0 = top;
            delta_top = top - cwawin->top;
            cwawin->top = top;
            cwawin->limit_top = -top;//����Ŀ���ʾ�ϱ߽�Ӧ�øպ�����ʾ�����ϱ߽�
        }

        if(right < disp->width)     //�����ұ߽���븲����ʾ��
        {
            cwawin->right = disp->width;
        }
        else
        {
            cwawin->right = right;
        }
        //����Ŀ���ʾ�ұ߽�Ӧ�øպ�����ʾ�����ұ߽�
        cwawin->limit_right = cwawin->limit_left + disp->width;

        if(bottom < disp->height)     //�����ұ߽���븲����ʾ��
        {
            cwawin->bottom = disp->height;
        }
        else
        {
            cwawin->bottom = bottom;
        }
        //����Ŀ���ʾ�±߽�Ӧ�øպ�����ʾ�����±߽�
        cwawin->limit_bottom = cwawin->limit_top + disp->height;
    }else
    {
        delta_left = para->left-cwawin->left;
        delta_top = para->top-cwawin->top;
        cwawin->absx0 += delta_left;//�޸Ĵ��ھ�������
        cwawin->absy0 += delta_top;

        cwawin->left = left;                      //�޸Ĵ����������
        cwawin->top = top;
        cwawin->right = right;
        cwawin->bottom = bottom;
        __GK_SetBound(cwawin);                        //���ÿ���ʾ�߽�
    }

    cwawin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_ALL;

    changing = cwawin->HostObj;
    current = OBJ_ForeachScion(changing, changing);
    //����gkwin���������ﴰ��,������ɺ�current��=NULL��
    while(current != NULL)
    {
        temp = (struct GkWinObj*)OBJ_GetPrivate(current);
        temp->absx0 += delta_left;        //�޸Ĵ��ھ�������
        temp->absy0 += delta_top;
        current = OBJ_ForeachScion(changing, current);
    }
    current = OBJ_ForeachScion(changing, changing);
    //��������α���������ȷ��Set �ﴰ��ʱ���Ӵ����Ѿ��ĺ���absx0
    while(current != NULL)
    {
        temp = (struct GkWinObj*)OBJ_GetPrivate(current);
        __GK_SetBound(temp);
        temp->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_ALL;
        current = OBJ_ForeachScion(changing, current);
    }
    __GK_ScanNewVisibleClip(cwawin->disp);
    cwawin->disp->reset_clip = true;
    return true;
}

//----���̴���-----------------------------------------------------------------
//���ܣ���һ��������ͬ�����ﴰ�ڴ��丸�������Ƴ�������������ڵ��Ӵ��ڡ����ڵ�
//      �������Բ��䡣����λ�õ�Z�������У��������ȼ���ͬ�Ĵ��ڵ���ǰ�ˡ�
//������gkwin��Ŀ�괰��
//      NewParent���µĸ�����
//���أ���
//-----------------------------------------------------------------------------
void __GK_AdoptWin(struct GkscParaAdoptWin *para)
{
    struct GkWinObj *last,*foremost,*gkwin,*Ztarget,*NowWin;
    struct Object *parent,*point;
    struct DisplayObj *display;
    gkwin = para->gkwin;
    parent = para->NewParent->HostObj;
    display = gkwin->disp;
    if(OBJ_GetParent(gkwin->HostObj) == parent)  //�µĸ�����û�ı�
        return ;

    //��gkwin�������Ӵ��ڣ������Ӵ���һ����Z����ռ��������һ�Σ�
    //ȡgkwin��Z������ǰ�����˵Ĵ���
    last = __GK_GetZsectionStart(gkwin);
    foremost = __GK_GetZsectionEnd(gkwin);

    //���¹��̴��������С�
    if(OBJ_GetChild(parent) == NULL)
    {
        OBJ_InsertToChild(parent,gkwin->HostObj);
        NowWin = (struct GkWinObj *)OBJ_GetPrivate(parent);
        if(gkwin->WinProperty.Zprio > 0)
            Ztarget = NowWin->z_back;
        else
            Ztarget = NowWin;
    }
    else    //�µĴ������Ӵ��ڣ�Ҫ�����´��ڵĲ����
    {
        //��һ��Z���ȼ��������´��ڵĴ���
        point = OBJ_ForeachChild(parent,parent);
        while( point != NULL )
        {
            NowWin = (struct GkWinObj *)OBJ_GetPrivate(point);
            if(NowWin->WinProperty.Zprio >= gkwin->WinProperty.Zprio)
                break;
            point = OBJ_ForeachChild(parent,point);
        }
        if(point == NULL)   //û���ҵ�Z���ȼ�����gkwin�Ĵ��ڣ�����ѭ���������
                            //�˴��ڵĺ��棬Ҳ������ǰ�˴��ڵ�ǰ��
        {
            point = OBJ_GetChild(parent);
        }
        OBJ_InsertToPrev(point, gkwin->HostObj); //�Ƶ�����������ʵ�λ��

        //��gkwin�Ƿ���Ϊ�����ڵ�ͷ���
        if((gkwin->WinProperty.Zprio <= 0) && (point == OBJ_GetChild(parent)))
        {
            OBJ_ChildMoveToOrev(parent);
        }

        //���²���Z����еĲ���㣬�������point���ڴ��ڶε�ǰ�ˣ���Ҫ�ж��Ƿ�
        //��Խparent
        NowWin = (struct GkWinObj *)OBJ_GetPrivate(point);
        if( (NowWin->WinProperty.Zprio > 0) && (gkwin->WinProperty.Zprio <= 0) )
            Ztarget = (struct GkWinObj *)parent;       //�������ڲ�����gkwin֮��
        else
            Ztarget = __GK_GetZsectionEnd( NowWin );
    }
    foremost->z_top->z_back = last->z_back;
    last->z_back->z_top = foremost->z_top;
    //�� Ztarget->z_top==foremos==lasttʱ�������������
    //��Ϊ���Ƴ��ڲ���
    Ztarget->z_top->z_back = foremost;
    foremost->z_top = Ztarget->z_top;
    Ztarget->z_top = last;
    last->z_back = Ztarget;
    __GK_ScanNewVisibleClip(display);
    display->reset_clip = true;
}

//----�ƶ�����-----------------------------------------------------------------
//����: �ı䴰���ڸ������ڵ����λ�ã������Ӵ��ڵ�����������ڸ����ڵģ����ƶ���
//      ��ʱ�����Ӵ���һ���ƶ�����������gk_change_win_area������һ��������
//����: gcwin��Ŀ�괰��
//      left��top���µ����Ͻ����꣬����ڸ�����
//����: ��
//-----------------------------------------------------------------------------
void __GK_MoveWin(struct GkscParaMoveWin *para)
{
    s32 delta_top,delta_left;
    struct GkWinObj *movewin;
    struct Object *moving,*current;
    struct DisplayObj *disp;
    movewin = para->gkwin;
    if((para->left == movewin->left) && (para->top == movewin->top))
        return;                                 //λ��û���޸ģ�ֱ�ӷ��ء�
    disp = movewin->disp;
    if(movewin == disp->desktop)
    {   //Ŀ�괰�������棬�ж�������:������븲����ʾ������
        if(para->left > 0)    //������߽���븲����ʾ��
        {
            movewin->absx0 = 0;
            delta_left = 0 - movewin->left;
            movewin->left = 0;
            movewin->limit_left = 0;
        }
        else
        {
            movewin->absx0 = para->left;
            delta_left = para->left - movewin->left;
            movewin->left = para->left;
            movewin->limit_left = -para->left;//����Ŀ���ʾ��߽�Ӧ�øպ�����ʾ������߽�
        }
        if(para->top > 0)    //�����ϱ߽���븲����ʾ��
        {
            movewin->absy0 = 0;
            delta_top = 0 - movewin->top;
            movewin->top = 0;
            movewin->limit_top = 0;
        }
        else
        {
            movewin->absy0 = para->top;
            delta_top = para->top - movewin->top;
            movewin->top = para->top;
            movewin->limit_top = -para->top;//����Ŀ���ʾ�ϱ߽�Ӧ�øպ�����ʾ�����ϱ߽�
        }

        movewin->right += delta_left;
        if(movewin->right < disp->width)     //�����ұ߽���븲����ʾ��
        {
            movewin->right = disp->width;
        }
        //����Ŀ���ʾ�ұ߽�Ӧ�øպ�����ʾ�����ұ߽�
        movewin->limit_right = movewin->limit_left + disp->width;

        movewin->bottom += delta_top;
        if(movewin->bottom < disp->height)     //�����ұ߽���븲����ʾ��
        {
            movewin->bottom = disp->height;
        }
        //����Ŀ���ʾ�±߽�Ӧ�øպ�����ʾ�����±߽�
        movewin->limit_bottom = movewin->limit_top + disp->height;
    }
    else
    {
        delta_left = para->left-movewin->left;
        delta_top = para->top-movewin->top;
        movewin->absx0 += delta_left;//�޸Ĵ��ھ�������
        movewin->absy0 += delta_top;

        movewin->left = para->left;                      //�޸Ĵ����������
        movewin->top = para->top;
        movewin->right += delta_left;
        movewin->bottom += delta_top;
//      if((movewin->absx0 <0) || (movewin->absy0 <0))
//          delta_left = 0;
        __GK_SetBound(movewin);                        //���ÿ���ʾ�߽�
    }
    moving = movewin->HostObj;
    current = OBJ_ForeachScion(moving, moving);
    //����gkwin���������ﴰ��
    while(current != NULL)
    {
        movewin = (struct GkWinObj*)OBJ_GetPrivate(current);
        movewin->absx0 += delta_left;        //�޸Ĵ��ھ�������
        movewin->absy0 += delta_top;
        current = OBJ_ForeachScion(moving, current);
    }
    current = OBJ_ForeachScion(moving, moving);
    //��������α���������ȷ��Set �ﴰ��ʱ���Ӵ����Ѿ��ĺ���absx0
    while(current != NULL)
    {
        movewin = (struct GkWinObj*)OBJ_GetPrivate(current);
        __GK_SetBound(movewin);
        movewin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_ALL;
        current = OBJ_ForeachScion(moving, current);
    }
    __GK_ScanNewVisibleClip(movewin->disp);
    movewin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_ALL;
    movewin->disp->reset_clip = true;
}

//----���ÿ���ʾ�߽�-----------------------------------------------------------
//����: �κδ��ڣ������ʹ�����Ǳ�z��ǰ�˴��ڼ��У��䴰�ڿ���ʾ��ΧҲ�����޵ģ�
//      ��bound_limit=true�ġ�������������ȴ��ڵĿ���ʾ�߽����ơ�����λ�øı�
//      ����Ҫ���ÿ���ʾ�߽硣
//      gkwin�������õĴ���
//����: ��
//-----------------------------------------------------------------------------
void __GK_SetBound(struct GkWinObj *gkwin)
{
    struct GkWinObj *ancestor;
    s32 gkwin_absx,gkwin_absy,ancestor_absx,ancestor_absy;
    ancestor = gkwin;
    //��ȡ���ȴ����е�һ���߽��ܸ��������Ƶĵ�һ�����ڣ�gkwin�ı߽��ܸô����޶�
    //���洰��һ���Ǹ��߽����ȴ���
    while(ancestor->WinProperty.BoundLimit == CN_BOUND_UNLIMIT)
    {
        ancestor = (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetParent(ancestor->HostObj));
    }
    ancestor = (struct GkWinObj*)OBJ_GetPrivate(OBJ_GetParent(ancestor->HostObj));
    if( (ancestor->limit_right  == 0) || (ancestor->limit_bottom == 0)    )
    {
        gkwin->limit_left   =0;
        gkwin->limit_top    =0;
        gkwin->limit_right  =0;
        gkwin->limit_bottom =0;
        return ;
    }

    gkwin_absx = gkwin->absx0;  //ȡ�����ڵľ������꣬Ҳ��gkwin->left�ľ�������
    gkwin_absy = gkwin->absy0;
    //ȡ���Ʊ����ڱ߽�Ĵ��ڵĿɼ��߽��������
    ancestor_absx = ancestor->absx0 +ancestor->limit_left;
    ancestor_absy = ancestor->absy0 +ancestor->limit_top;


    if(gkwin_absx < ancestor_absx)
       gkwin->limit_left = ancestor_absx - gkwin_absx;
    else
        gkwin->limit_left = 0;

    if(gkwin_absy < ancestor_absy)
       gkwin->limit_top = ancestor_absy - gkwin_absy;
    else
        gkwin->limit_top = 0;

    if((gkwin_absx+gkwin->right-gkwin->left)
            < (ancestor_absx+ancestor->limit_right))
        gkwin->limit_right = gkwin->right - gkwin->left;
    else
        gkwin->limit_right = ancestor_absx + ancestor->limit_right - gkwin->left;

    if((gkwin_absy+gkwin->bottom-gkwin->top)
            < (ancestor_absy+ancestor->limit_bottom))
        gkwin->limit_bottom = gkwin->bottom - gkwin->top;
    else
        gkwin->limit_bottom = ancestor_absy + ancestor->limit_bottom - gkwin->top;

    if(         (gkwin->limit_left   > gkwin->right)
            ||  (gkwin->limit_top    > gkwin->bottom)
            ||  (gkwin->limit_right  < 0)
            ||  (gkwin->limit_bottom < 0)    )
    {
        gkwin->limit_left   =0;
        gkwin->limit_top    =0;
        gkwin->limit_right  =0;
        gkwin->limit_bottom =0;
    }
}
//----���ñ߽�ģʽ-------------------------------------------------------------
//����: �趨���ڵ���ʾ�߽��Ƿ��ܸ��������ƣ����ƺ��Ӵ��ڳ��������ڵĲ��ֽ�����
//      ��ʾ��desktop��ֱ���Ӵ���Ĭ�����ޣ����ܸ��ġ�
//����: gkwin��Ŀ�괰��
//      mode��trueΪ���ޣ�falseΪ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_SetBoundMode(struct GkscParaSetBoundMode *para)
{
    struct GkWinObj *current;
    struct Object *Scion,*ancestor;
    if(para->gkwin == NULL)
        return;
    if(para->gkwin == para->gkwin->disp->desktop)   //���治���޸ı߽�ģʽ
        return;
    if(para->gkwin->WinProperty.BoundLimit == para->mode)      //ģʽδ�ı�
        return;
    if(para->gkwin->disp->desktop->HostObj == OBJ_GetParent(para->gkwin->HostObj))
        return;                         //ֱ�ӷ���������Ӵ��ڣ�����ı�
    //Ŀ�괰�ڱ߽�ģʽ�ı䣬�������Ըı�
    para->gkwin->WinProperty.BoundLimit = CN_BOUND_LIMIT;
    __GK_SetBound(para->gkwin);
    ancestor = para->gkwin->HostObj;
    Scion = OBJ_ForeachScion(ancestor,ancestor);
    //����gkwin���������ﴰ��
    while( Scion != NULL)
    {
        current = (struct GkWinObj*)OBJ_GetPrivate(Scion);
        __GK_SetBound(current);
    }
    __GK_ScanNewVisibleClip(para->gkwin->disp);
    para->gkwin->disp->reset_clip = true;
}

//----���ô����Ƿ����---------------------------------------------------------
//���ܣ�����һ�����ڵĿ������ԣ����治������
//����: gkwin��Ŀ�괰��
//      visible��CN_GKWIN_VISIBLE=���ӣ�CN_GKWIN_HIDE=����
//���أ���
//-----------------------------------------------------------------------------
void __GK_SetVisible(struct GkscParaSetVisible *para)
{
    struct GkWinObj *gkwin;
    struct DisplayObj *display;

    gkwin = para->gkwin;
    display = para->gkwin->disp;
    if(gkwin == display->desktop)  //���洰�ڿ������Բ��ɸı�
        return;
    gkwin->WinProperty.Visible = para->Visible;

    __GK_ScanNewVisibleClip(gkwin->disp);
    gkwin->disp->reset_clip = true;

}

//----���ô�����ʾ���ȼ�-------------------------------------------------------
//����: ����һ�����ڵ����ȼ������ȼ�������ͬ�������е�z�����λ�ã�����ԽС����
//      z����Խ��ǰ�����ȼ�<=0�����Ǹ����ڣ���֮�������ڸ��ǡ����ڸı����ȼ���
//      ����Z���е�˳����ܻ�ı䣬��Ļ����Ҳ���ܻ�ı䡣
//      ���ڱ��ı����ȼ��Ĵ��ڿ��ܻ����Ӵ��ڣ����ԣ���z���б��ƶ��ģ�����һ����
//      �ڣ�����������һ�δ��ڡ�
//����: gkwin��Ŀ�괰��
//      prio�������ȼ�
//����: ��
//-----------------------------------------------------------------------------
void __GK_SetPrio(struct GkscParaSetPrio *para)
{
    struct GkWinObj *target_section,*gkwin;
    struct DisplayObj *display;
    struct GkWinObj *parent;       //gkwin�ĸ�����
    //z���б��ƶ��Ĵ��ڶ����˵Ĵ���
    struct GkWinObj *section_start;
    //z���б��ƶ��Ĵ��ڶ���ǰ�˵Ĵ���
    struct GkWinObj *section_end,*target_win;

    gkwin = para->gkwin;
    display = para->gkwin->disp;
    if(gkwin == display->desktop)  //���洰�ڵ����ȼ����ɸı�
        return;

    parent = (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetParent(gkwin->HostObj));
    //ȡz���б��ƶ��Ĵ��ڶ���ǰ�˵Ĵ��ڶ�
    section_end = (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetChild(parent->HostObj));
    section_start = (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetPrev(section_end->HostObj));
    target_section = section_end;
    while(1)
    {   //����ͬ�������к����ȼ�����para->prio�ĵĴ��ڣ�ɨ����z��Ϊ����
        //ɨ��˳��:��z���ǰ�����˽���ɨ�裬ֻɨ�ֵܴ��ڣ���ɨ�Ӵ���
        if(target_section->WinProperty.Zprio >= (s32)para->prio)
            break;
        //ɨ��Ŀ�괰������z������һ���ڣ���ֹ����
        if(target_section == section_start)
            break;
        target_section = (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetNext(target_section->HostObj));
    }
    if(target_section->WinProperty.Zprio >= (s32)para->prio)
    {   // �ҵ�gkwinͬ�����������ȼ����ڻ������prio�Ĵ���
        if((target_section == gkwin)
            || (target_section == (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetNext(gkwin->HostObj))))
        {
            //����Դ�����������ƶ��������ȼ����п���Ҫ�ƶ�
            if((gkwin->WinProperty.Zprio <= CN_ZPRIO_DEFAULT)
                    && (para->prio > CN_ZPRIO_DEFAULT))    //�Ƶ������ں��
            {
                //��ȡgkwin�������ﴰ������z��εĿ�ʼ�ͽ�������
                section_start = __GK_GetZsectionStart(gkwin);
                section_end = __GK_GetZsectionEnd(gkwin);
                //����gkwinҪ����Ĵ����Ƿ�����ǰ�˴���,
                //��,��gkwin����Z�����ǰ�˵Ĵ��ڵ���Ϊ����Z�����ǰ�˴���
                if(section_end == display->z_topmost)
                    display->z_topmost = parent;
                //��gkwin����z��δ�z��ȡ����
                section_start->z_back->z_top = section_end->z_top;
                section_end->z_top->z_back = section_start->z_back;

                section_start->z_top = parent;
                section_end->z_back = parent->z_back;
                parent->z_back->z_top = section_start;
                parent->z_back = section_end;

                gkwin->WinProperty.Zprio = para->prio;
                __GK_ScanNewVisibleClip(display);
                display->reset_clip = true;
            }
            else if((gkwin->WinProperty.Zprio > CN_ZPRIO_DEFAULT)
                            && (para->prio <= CN_ZPRIO_DEFAULT))//�Ƶ�������ǰ��
            {
                //��ȡgkwin�������ﴰ������z��εĿ�ʼ�ͽ�������
                section_start = __GK_GetZsectionStart(gkwin);
                section_end = __GK_GetZsectionEnd(gkwin);
                //����gkwinҪ����Ĵ����Ƿ�����ǰ�˴���,
                //��,��gkwin����Z�����ǰ�˵Ĵ��ڵ���Ϊ����Z�����ǰ�˴���
                if(parent == display->z_topmost)
                    display->z_topmost = section_end;
                //��gkwin����z��δ�z��ȡ����
                section_start->z_back->z_top = section_end->z_top;
                section_end->z_top->z_back = section_start->z_back;

                section_start->z_back = parent;
                section_end->z_top = parent->z_top;
                parent->z_top->z_back = section_end;
                parent->z_top = section_start;

                gkwin->WinProperty.Zprio = para->prio;
                __GK_ScanNewVisibleClip(display);
                display->reset_clip = true;
            }
            else    //��������������ƶ�z��
            {
                gkwin->WinProperty.Zprio = para->prio;
            }
        }
        else    //��Դ�������ƶ�
        {
            OBJ_InsertToPrev(target_section->HostObj, gkwin->HostObj);

            if((para->prio <= CN_ZPRIO_DEFAULT)
                    && (target_section->WinProperty.Zprio > CN_ZPRIO_DEFAULT))
                target_win = parent;
            else
                target_win = __GK_GetZsectionEnd(target_section);
            //��ȡgkwin�������ﴰ������z��εĿ�ʼ�ͽ�������
            section_start = __GK_GetZsectionStart(gkwin);
            section_end = __GK_GetZsectionEnd(gkwin);
            //����gkwinҪ����Ĵ����Ƿ�����ǰ�˴���,
            //��,��gkwin����Z�����ǰ�˵Ĵ��ڵ���Ϊ����Z�����ǰ�˴���
            if(target_win == display->z_topmost)
                display->z_topmost = section_end;
            //��gkwin����z��δ�z��ȡ����
            section_start->z_back->z_top = section_end->z_top;
            section_end->z_top->z_back = section_start->z_back;

            section_start->z_back = target_win;
            section_end->z_top = target_win->z_top;
            target_win->z_top->z_back = section_end;
            target_win->z_top = section_start;

            gkwin->WinProperty.Zprio = para->prio;
            __GK_ScanNewVisibleClip(display);
            display->reset_clip = true;
        }
    }
    else
    {//ͬ���������ȼ���������prio
        //OBJ_MoveToNext(target_section->HostObj,gkwin->HostObj);
        OBJ_InsertToNext(target_section->HostObj,gkwin->HostObj);
        if((para->prio > CN_ZPRIO_DEFAULT)
                    && (target_section->WinProperty.Zprio <=CN_ZPRIO_DEFAULT))
        {
            target_win = parent;
        }
        else
        {
            //�ҵ�new_location����������ɵ�z��������˵Ĵ���
            target_win = __GK_GetZsectionStart(target_section);
        }
        //���°�gkwinȡ��������move_end_temp���
        //��ȡgkwin�������ﴰ������z��εĿ�ʼ�ͽ�������
        section_start = __GK_GetZsectionStart(gkwin);
        section_end = __GK_GetZsectionEnd(gkwin);

        //��gkwin����z��δ�z��ȡ����
        section_start->z_back->z_top = section_end->z_top;
        section_end->z_top->z_back = section_start->z_back;

        section_start->z_top = target_win;
        section_end->z_back = target_win->z_back;
        target_win->z_back->z_top = section_start;
        target_win->z_back = section_end;

        gkwin->WinProperty.Zprio = para->prio;
        __GK_ScanNewVisibleClip(display);
        display->reset_clip = true;
    }
}

//----���ù�դ������---------------------------------------------------------
//����: �ı䴰�ڵĹ�դ�����롣�ǻ��崰�ں�����ֻ֧��CN_R2_COPYPEN��
//        �����޸Ĺ�դ�����롣
//����: para������
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t __GK_SetRopCode(struct GkscParaSetRopCode *para)
{
    struct GkWinObj *mygkwin;
    struct DisplayObj *mydisplay;

    mygkwin = para->gkwin;
    mydisplay = mygkwin->disp;
    //����ͷǻ��崰�ڲ����޸�RopCode
    if((mygkwin->wm_bitmap == NULL)
        || (mydisplay->desktop == mygkwin))
    {
        return false;
    }
    else if(memcmp(&para->RopCode, &mygkwin->RopCode,sizeof(struct RopGroup))==0)
    {
        return true;
    }
    else
    {
        mygkwin->WinProperty.DestBlend = __GK_RopIsNeedDestination(para->RopCode);
        mygkwin->RopCode = para->RopCode;
        mygkwin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_ALL;
//      mygkwin->RopCode &= ~CN_ROP_ROP2_MSK;   //�������Բ�֧��rop2
        return true;
    }
}

//----���ô���͸��ɫ-----------------------------------------------------------
//����: �ı䴰��͸��ɫ
//����: gkwin��Ŀ�괰��
//      transparentcolor�����õ�͸��ɫ,��ɫ��ʽ�����ɫ
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t __GK_SetHyalineColor(struct GkscParaSetHyalineColor *para)
{
    u32 color;
    //���治��ҪKeyColor
    if(para->gkwin->disp->desktop == para->gkwin)
        return false;
    if(para->gkwin->wm_bitmap == NULL)
        return false;
    color = GK_ConvertRGB24ToPF(para->gkwin->wm_bitmap->PixelFormat,para->HyalineColor);
    para->gkwin->HyalineColor =
        GK_ConvertColorToRGB24(para->gkwin->wm_bitmap->PixelFormat, color, para->gkwin->wm_bitmap->ExColor);

    if((para->gkwin->RopCode.HyalineEn == 1)
       && (para->gkwin->HyalineColor == para->HyalineColor))
    {
        //�������κ���
    }
    else
    {
        para->gkwin->RopCode.HyalineEn = 1;
        para->gkwin->WinProperty.DestBlend = CN_GKWIN_DEST_VISIBLE;
        para->gkwin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_ALL;
    }
    return true;
}
//----���ô���ֱ��д������-----------------------------------------------------
//����: ���ô���ֱ��д�����ԡ�
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void __GK_SetDirectScreen(struct GkWinObj *gkwin)
{
    gkwin->WinProperty.DirectDraw = CN_GKWIN_DIRECT_DRAW;
}
//----��������ֱ��д������-----------------------------------------------------
//����: ��������ֱ��д�����ԡ�
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void __GK_SetUnDirectScreen(struct GkWinObj *gkwin)
{
    gkwin->WinProperty.DirectDraw = CN_GKWIN_UNDIRECT_DRAW;
}
//----���ٴ���(�Ӵ���)---------------------------------------------------------
//����: �ͷŴ�����ռ�õ���Դ��
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void __gk_destroy_win(struct GkWinObj *gkwin)
{
    if(gkwin->disp->z_topmost == gkwin)
        gkwin->disp->z_topmost = gkwin->z_back;
    gkwin->z_back->z_top = gkwin->z_top;
    gkwin->z_top->z_back = gkwin->z_back;
    OBJ_Delete(gkwin->HostObj);
    if(gkwin->visible_clip != NULL)
        gkwin->disp->reset_clip = true;
    gkwin->visible_clip = __GK_FreeClipQueue(gkwin->visible_clip);
    gkwin->visible_bak =  __GK_FreeClipQueue(gkwin->visible_bak);
    if(gkwin->disp->frame_buffer != NULL)
    {
        if(gkwin->wm_bitmap != NULL)//��֡�����ڻ���ģʽ��
        {
            __gk_vfree(gkwin->disp,gkwin);
            M_FreeHeap(gkwin->changed_msk.bm_bits,gkwin->disp->DisplayHeap);
            return;
        }
        else
            return;
    }
    else
        return;
}
//----���ٴ���-------------------------------------------------------------
//����: �ͷŴ�����ռ�õ���Դ�����ٴ��ڣ�ͬʱ���ٴ��ڵ����ﴰ�ڡ�
//����: gkwin��Ŀ�괰��
//����: ��
//-----------------------------------------------------------------------------
void __GK_DestroyWin(struct GkWinObj *gkwin)
{
    struct GkWinObj *CurWin;
    while((CurWin = (struct GkWinObj *)OBJ_GetPrivate((struct Object*)OBJ_GetTwig(gkwin->HostObj))) != NULL)
    {
        __gk_destroy_win(CurWin);
        M_FreeHeap(CurWin,CurWin->disp->DisplayHeap);
    }
    __gk_destroy_win(gkwin);
    M_FreeHeap(gkwin,gkwin->disp->DisplayHeap);
    if(gkwin->disp->reset_clip == true)
    {
        __GK_ScanNewVisibleClip(gkwin->disp);
    }
}

//----�������redraw����-------------------------------------------------------
//����: �����д������޸ĵĿ������������ʾ��������������Ѿ�׼���á�
//����: display����ɨ�����ʾ��
//����: ��
//-----------------------------------------------------------------------------
void __GK_OutputRedraw(struct DisplayObj *display)
{
    s32 x,y,x_src,y_src,x_dst,y_dst;
    u32 color;
    struct ClipRect *clip;
    struct DisplayObj *MirrorDisplay;
    struct Object *mirror,*current;
    struct GkWinObj *gkwin,*desktop_gkwin,*topwin,*frame_buf;
    struct GkucParaRepaint repaint;
    struct RectBitmap *src,*dst;
    struct Rectangle *rect,SrcRect;
    struct RectBitmap *bitmap;

    frame_buf = display->frame_buffer;          //ȡ֡���崰��
    desktop_gkwin = display->desktop;
    topwin = display->z_topmost;
    if(frame_buf == NULL)   //��֡���壬��֡�������ʾ������Ȼû�д��ڻ���
    {
        //��û�л�����������£����Ʋ����ǲ�������redraw_clip�ģ�ֻ�д��ڹ���
        //�����Ż�Ҫ���ػ档
        gkwin = desktop_gkwin;
        while(1)
        {
            clip = gkwin->redraw_clip;
            if(clip != NULL)
            {
                repaint.command = CN_GKUC_REPAINT;
                repaint.gk_win = (void*)gkwin;
                repaint.redraw_clip = clip;

                __GK_UsercallChunnel((void*)&repaint,sizeof(struct GkucParaRepaint),
                                     1000*mS);

                gkwin->redraw_clip = __GK_FreeClipQueue(clip);
            }
            if(gkwin != topwin)
                gkwin = gkwin->z_top;
            else
                break;
        }
    }else                                           //��֡����
    {
        gkwin = desktop_gkwin;
        while(1)
        {
            clip = gkwin->redraw_clip;
            if(clip != NULL)
            {
                if(gkwin->wm_bitmap == NULL)   //�޴��ڻ���
                {
                    repaint.command = CN_GKUC_REPAINT;
                    repaint.gk_win = (void*)gkwin;
                    repaint.redraw_clip = clip;

                    __GK_UsercallChunnel((void*)&repaint,sizeof(struct GkucParaRepaint),
                                            1000*mS);

                    //�ȴ��û�����ػ��������ʱ(1��)����
                }else                                   //�д��ڻ���
                {
                    do
                    {
                        SrcRect.left = clip->rect.left-gkwin->absx0;
                        SrcRect.top = clip->rect.top-gkwin->absy0;
                        SrcRect.right = clip->rect.right -gkwin->absx0;
                        SrcRect.bottom = clip->rect.bottom -gkwin->absy0;
                        if(!display->draw.BltBitmapToBitmap(
                                            frame_buf->wm_bitmap,
                                            &clip->rect,
                                            gkwin->wm_bitmap,
                                            &SrcRect,
                                            gkwin->RopCode,
                                            gkwin->HyalineColor))
                        {
                            dst = frame_buf->wm_bitmap;
                            src = gkwin->wm_bitmap;

                            y_dst = clip->rect.top;
                            if(gkwin->WinProperty.DestBlend == CN_GKWIN_DEST_HIDE)
                            {
                                for(y_src = clip->rect.top-gkwin->absy0;
                                    y_src < clip->rect.bottom-gkwin->absy0;
                                    y_src++)
                                {
                                    x_dst = clip->rect.left;
                                    for(x_src = clip->rect.left-gkwin->absx0;
                                        x_src < clip->rect.right-gkwin->absx0;
                                        x_src++)
                                    {
                                        __GK_CopyPixelBm(dst,src,x_dst,y_dst,
                                                            x_src,y_src);
                                        x_dst ++;
                                    }
                                    y_dst ++;
                                 }
                            }
                            else
                            {
                                for(y_src = clip->rect.top-gkwin->absy0;
                                    y_src < clip->rect.bottom-gkwin->absy0;
                                    y_src++)
                                {
                                    x_dst = clip->rect.left;
                                    for(x_src = clip->rect.left-gkwin->absx0;
                                        x_src < clip->rect.right-gkwin->absx0;
                                        x_src++)
                                    {
                                        __GK_CopyPixelRopBm(dst,src,
                                                x_dst,y_dst,x_src,y_src,
                                                gkwin->RopCode,gkwin->HyalineColor);
                                        x_dst ++;
                                    }
                                    y_dst ++;
                                }
                            }
                        }
                        __GK_ShadingRect(frame_buf,&clip->rect);
                        clip = clip->next;
                    }while(clip != gkwin->redraw_clip);
                }
                gkwin->redraw_clip = __GK_FreeClipQueue(gkwin->redraw_clip);
            }
            if(gkwin != topwin)
                gkwin = gkwin->z_top;
            else
                break;
        }
        //���°�֡�������������ʾ��
        clip = __GK_GetChangedClip(frame_buf);  //��ȡ֡�����б��޸ĵĿ�����
        if(clip != NULL)
        {
            __GK_ShadingClear(frame_buf);
            frame_buf->redraw_clip = clip;
//          mirror = gkwin->disp->HostObj;
            do{
                if(display->framebuf_direct == false)
                {
                    //�����screen
                    if(display->draw.CopyBitmapToScreen(&clip->rect,
                                                frame_buf->wm_bitmap,
                                                clip->rect.left,clip->rect.top))
                    {
                    }
                    else
                    {
                        rect = &clip->rect;
                        bitmap = frame_buf->wm_bitmap;
                        for(y = rect->top;y < rect->bottom;y++)
                        {
                            for(x = rect->left;x < rect->right;x++)
                            {
                                color = GK_GetPixelBm(bitmap,x,y);
                                color =GK_ConvertColorToRGB24(bitmap->PixelFormat,
                                                         color,bitmap->ExColor);
                                display->draw.SetPixelToScreen(x,y,color,
                                                                CN_R2_COPYPEN);
                            }
                        }
                    }
                }

                //��������ʾ
                mirror = display->HostObj;
                current = OBJ_GetChild(mirror);
                while(current != NULL)
                {
                    MirrorDisplay = (struct DisplayObj*)OBJ_GetPrivate(current);
                    MirrorDisplay->draw.CopyBitmapToScreen(&clip->rect,
                                                frame_buf->wm_bitmap,
                                                clip->rect.left,clip->rect.top);
                    current = OBJ_ForeachChild(mirror, current);
                }
                clip = clip->next;
            }while(clip != frame_buf->redraw_clip);
            frame_buf->redraw_clip =
                            __GK_FreeClipQueue(frame_buf->redraw_clip);
        }
    }
}

//----��ʾ�����������ʾ��-----------------------------------------------------
//����: ��������ʾ������Ҫ����Ĳ����������ʾ����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void __gk_redraw_all(void)
{
    struct DisplayObj *Display;
    struct Object *current;

    current = OBJ_GetChild(s_ptDisplayDir);
    while(current != NULL)
    {
        Display = (struct DisplayObj*)OBJ_GetPrivate(current);
        __GK_GetRedrawClipAll(Display);//ɨ��ȡ���µ����ػ������
        __GK_OutputRedraw(Display);    //�ػ�
        current = OBJ_ForeachChild(s_ptDisplayDir, current);
    }

}

//----����ˢ����ʾ��----------------------------------------------------------
//����: ��������GK_ApiRefreshDisplay�������𡣶�����framebuffer����ʾ��,������
//      ��ʵ���൱��,���ǰ�framebuffer����copy��display��mirror����.����
//      framebuffer����ʾ��,����΢����һЩ,��Ҫ�Ѹ����ڵĿ�������Ϊ�ػ���ͨ��
//      GK_UsercallChunnel��������Ӧ�ò�, ����Ӧ�ò��ػ档
//      �����������,�����������ı����д��ڵĿ��������ػ���.
//����: Display,��ˢ�µ���ʾ��
//����: ��
//-----------------------------------------------------------------------------
void __gk_RefreshDisplay(struct DisplayObj *Display)
{
    s32 x,y;
    u32 color;
    struct ClipRect *clip;
    struct DisplayObj *MirrorDisplay;
    struct Object *mirror,*current;
    struct GkWinObj *gkwin,*desktop_gkwin,*topwin,*frame_buf;
    struct GkucParaRepaint repaint;
    struct Rectangle rect;
    struct RectBitmap *bitmap;

    frame_buf = Display->frame_buffer;          //ȡ֡���崰��
    desktop_gkwin = Display->desktop;
    topwin = Display->z_topmost;
    if(frame_buf == NULL)   //��֡���壬��֡�������ʾ������Ȼû�д��ڻ���
    {
        //��û�л�����������£�ֱ�Ӱ�visible_clip��Ϊ�ػ����ϴ���
        gkwin = desktop_gkwin;
        while(1)
        {
            clip = gkwin->visible_clip;
            if(clip != NULL)
            {
                repaint.command = CN_GKUC_REPAINT;
                repaint.gk_win = (void*)gkwin;
                repaint.redraw_clip = clip;
                __GK_UsercallChunnel((void*)&repaint,sizeof(struct GkucParaRepaint),
                                        1000*mS);
                //�ȴ��û�����ػ��������ʱ(1��)����
//              Lock_SempPend(g_ptUsercallSemp,0);
            }
            if(gkwin != topwin)
                gkwin = gkwin->z_top;
            else
                break;
        }
    }else                                           //��֡����
    {
        rect.left = 0;
        rect.top = 0;
        rect.right = Display->width - 1;
        rect.bottom = Display->height - 1;
        //���°�֡�������������ʾ��
        if(Display->framebuf_direct == false)
        {
            //�����screen
            if(Display->draw.CopyBitmapToScreen(&rect,frame_buf->wm_bitmap,0,0))
            {
            }
            else
            {
                bitmap = frame_buf->wm_bitmap;
                for(y = rect.top;y < rect.bottom;y++)
                {
                    for(x = rect.left;x < rect.right;x++)
                    {
                        color = GK_GetPixelBm(bitmap,x,y);
                        color =GK_ConvertColorToRGB24(bitmap->PixelFormat,
                                                 color,bitmap->ExColor);
                        Display->draw.SetPixelToScreen(x,y,color,
                                                        CN_R2_COPYPEN);
                    }
                }
            }
        }
        //��������ʾ
        mirror = Display->HostObj;
        current = OBJ_GetChild(mirror);
        while(current != NULL)
        {
            MirrorDisplay = (struct DisplayObj*)OBJ_GetPrivate(current);
            MirrorDisplay->draw.CopyBitmapToScreen(&rect,frame_buf->wm_bitmap,0,0);
            current = OBJ_ForeachChild(mirror, current);
        }

    }
}

//----ϵͳ���ùܵ�-------------------------------------------------------------
//����: gui api��gui kernel�Ľӿں�������api������Ļ���Ҫ�󴫵ݵ�gui kernel����
//      mp�汾�У����Կ���ʹ�����ж�ʵ�֡�
//����: param��������ַ
//      size����������
//����: ʵ��д��ܵ�����������0����size��
//-----------------------------------------------------------------------------
extern bool_t WaitGkDraw(u32 timeout);
u16 __GK_SyscallChunnel(u16 command,u32 sync_time,void *param1,u16 size1,
                                                void *param2,u16 size2)
{
    u16 completed = 0;
    u8 buf[4];
    u32 base_time,rel_timeout = sync_time;
    base_time = (u32)DJY_GetSysTime();
    //�ܵ����ʻ��⣬���ڶ���ϲ�Ӧ�ò�������֮��Ļ���
    if(Lock_MutexPend(g_tGkChunnel.syscall_mutex,rel_timeout))
    {
        while(1)
        {
            if((Ring_Capacity(&g_tGkChunnel.ring_syscall)
                        - Ring_Check(&g_tGkChunnel.ring_syscall)) <(u32)(size1+size2+4))
            {
                //�ܵ��������㣬��ִ�йܵ��ڵ��������gkserver�����ȼ�����������ִ�У���
                //���߳�Ҳ���ھ���̬��gkserver��ִ��һ��ʱ��Ƭ�󽫷��أ���ʱ�����Ƿ�ִ����ϣ�
                //����gkserver�ָ̻߳������ȼ���

                //�ϲ�Ӧ�ö���·������gk_server��һ�δ���������һ�£�����gk_server
                //��ת
                if(Lock_SempQueryFree(g_ptSyscallSemp) == 0)
                    Lock_SempPost(g_ptSyscallSemp);
                WaitGkDraw(1);

                continue;       //�ٴμ��ܵ�����
            }
            buf[0] = (u8)command;
            buf[1] = (u8)(command>>8);
            buf[2] = (u8)((size1+size2)&0xff);
            buf[3] = (u8)(((size1+size2)>>8)&0xff);

            completed = Ring_Write(&g_tGkChunnel.ring_syscall,buf,4);
            if(size1 != 0)
                completed += Ring_Write(&g_tGkChunnel.ring_syscall,param1,size1);
            if(size2 != 0)
                completed += Ring_Write(&g_tGkChunnel.ring_syscall,param2,size2);
            break;
        }
        if(DJY_IsMultiEventStarted())
        {
            //�ϲ�Ӧ�ö���·������gk_server��һ�δ���������һ�£�������post����
            //gk_server����������
            if(Lock_SempQueryFree(g_ptSyscallSemp) == 0)
                Lock_SempPost(g_ptSyscallSemp);
            if(0 != sync_time)      //�趨�˵ȴ�ʱ��
            {
                rel_timeout = (u32)DJY_GetSysTime() - base_time;
                if(rel_timeout < sync_time)
                {
                    rel_timeout = sync_time - rel_timeout;
                    WaitGkDraw(rel_timeout);
                }
            }
        }
        else
        {
            completed =  Ring_Read(&g_tGkChunnel.ring_syscall,(u8*)draw_chunnel_buf,
                                            CFG_GKERNEL_CMD_DEEP);
            __ExecOneCommand(command,(u8 *)draw_chunnel_buf+4);
        }
        Lock_MutexPost(g_tGkChunnel.syscall_mutex);    //�ܵ����ʻ�����
    }
    else
        completed = 0;
    return completed;
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----Usercall��Ϣ����----------------------------------------------------------
//����: ��������gk_syscall_chunnle�������������������gui kernel��Ҫ�ϲ�Ӧ�ó���
//      ���ĳЩ����ʱ�����ñ��������Ѳ���Ҫ��ͨ���ܵ�������ȥ��
//����: usercall_id: ��ϢID
//      pdata: ��Ϣ����
//      size: ��Ϣ���������ֽ���(��������ϢID��ռ�ֽ�)
//����: ture:�ɹ�; false:ʧ��
//-----------------------------------------------------------------------------
bool_t  __GK_UsercallChunnel(void *pdata,u16 size,u32 timeout)
{
//  printf("call usercall\r\n");
//  u8 buf[CN_USERCALL_MSG_SIZE];
//
//  if((size + 4) > CN_USERCALL_MSG_SIZE)
//  {
//      return false;
//  }
//
//  memcpy(&buf[0],&usercall_id,2);
//  memcpy(&buf[2],&size,2);
//  *(u16 *)&buf[0] = usercall_id;
//  *(u16 *)&buf[2] = size;
//  memcpy(&buf[4],pdata,size);

//  return  MsgQ_Send(g_tGkChunnel.usercall_msgq,pdata,size,timeout,CN_MSGQ_PRIO_NORMAL);
    return false;
}
#pragma GCC diagnostic pop

//����: �����(�ֽ���)
u32 __ExecOneCommand(u16 DrawCommand,u8 *ParaAddr)
{
    u32 result = 0;
    switch(DrawCommand)
    {
        case CN_GKSC_CREAT_GKWIN:
        {
            struct GkscParaCreateGkwin para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaCreateGkwin));
            *(para.result) = __GK_CreateWin(&para);
            result = sizeof(struct GkscParaCreateGkwin);
        } break;
        case CN_GKSC_SET_PIXEL:
        {
            struct GkscParaSetPixel para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaSetPixel));
            __GK_SetPixel(&para);
            result = sizeof(struct GkscParaSetPixel);
        } break;
        case CN_GKSC_LINETO:
        {
            struct GkscParaLineto para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaLineto));
            __GK_Lineto(&para);
            result = sizeof(struct GkscParaLineto);
        } break;
        case CN_GKSC_LINETO_INC_END:
        {
            struct GkscParaLineto para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaLineto));
            __GK_LinetoIe(&para);
            result = sizeof(struct GkscParaLineto);
        } break;
        case CN_GKSC_DRAW_BITMAP_ROP:
        {
            struct GkscParaDrawBitmapRop para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaDrawBitmapRop));
            __GK_DrawBitMap(&para);
            result = sizeof(struct GkscParaDrawBitmapRop);
        } break;
        case CN_GKSC_FILL_WIN:
        {
            struct GkscParaFillWin para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaFillWin));
            __GK_FillWin(&para);
            result = sizeof(struct GkscParaFillWin);
        } break;
#if 0
        case CN_GKSC_FILL_PART_WIN:
        {
            struct GkscParaFillPartWin para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaFillPartWin));
            GK_FillPartWin(&para);
            result = sizeof(struct GkscParaFillPartWin);
        } break;
#endif
        case CN_GKSC_FILL_RECT:
        {
            struct GkscParaGradientFillWin para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaGradientFillWin));
            __GK_GradientFillRect(&para);
            result = sizeof(struct GkscParaGradientFillWin);
        } break;
        case CN_GKSC_SET_ROP_CODE:
        {
            struct GkscParaSetRopCode para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaSetRopCode));
            __GK_SetRopCode(&para);
            result = sizeof(struct GkscParaSetRopCode);
        } break;
        case CN_GKSC_DRAW_TEXT:
        {
            struct GkscParaDrawText para;
            u32 len;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaDrawText));
            result = sizeof(struct GkscParaDrawText);
            ParaAddr+=result;
            __GK_DrawText(&para,(char*)ParaAddr,&len);
            result += strlen((char*)ParaAddr)+ Charset_Get_EOC_Size(para.pCharset);
        }break;
        case CN_GKSC_SET_HYALINE_COLOR:
        {
            struct GkscParaSetHyalineColor para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaSetHyalineColor));
            __GK_SetHyalineColor(&para);
            result = sizeof(struct GkscParaSetHyalineColor);
        } break;
        case CN_GKSC_DESTROY_WIN:
        {
            struct GkWinObj *gkwin;
            memcpy(&gkwin,ParaAddr,sizeof(struct GkWinObj *));
            __GK_DestroyWin(gkwin);
            result = sizeof(struct GkWinObj *);
        } break;
        case CN_GKSC_SET_PRIO:
        {
            struct GkscParaSetPrio para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaSetPrio));
            __GK_SetPrio(&para);
            result = sizeof(struct GkscParaSetPrio);
        } break;
        case CN_GKSC_SET_VISIBLE:
        {
            struct GkscParaSetVisible para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaSetVisible));
            __GK_SetVisible(&para);
            result = sizeof(struct GkscParaSetVisible);
        } break;
        case CN_GKSC_SET_BOUND_MODE:
        {
            struct GkscParaSetBoundMode para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaSetBoundMode));
            __GK_SetBoundMode(&para);
            result = sizeof(struct GkscParaSetBoundMode);
        }break;
        case CN_GKSC_ADOPT_WIN:
        {
            struct GkscParaAdoptWin para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaAdoptWin));
            __GK_AdoptWin(&para);
            result = sizeof(struct GkscParaAdoptWin);
        }break;
        case CN_GKSC_MOVE_WIN:
        {
            struct GkscParaMoveWin para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaMoveWin));
            __GK_MoveWin(&para);
            result = sizeof(struct GkscParaMoveWin);
        }break;
        case CN_GKSC_CHANGE_WIN_AREA:
        {
            struct GkscParaChangeWinArea para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaChangeWinArea));
            __GK_ChangeWinArea(&para);
            result = sizeof(struct GkscParaChangeWinArea);
        }break;
        case CN_GKSC_DRAW_CIRCLE:
        {
            struct GkscParaDrawCircle para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaDrawCircle));
            __GK_DrawCircle(&para);
            result = sizeof(struct GkscParaDrawCircle);
        } break;
        case CN_GKSC_BEZIER:
        {
            struct GkscParaBezier para;
            memcpy(&para,ParaAddr,sizeof(struct GkscParaBezier));
            __GK_Bezier(&para);
            result = sizeof(struct GkscParaBezier);
        } break;
        case CN_GKSC_SET_DIRECT_SCREEN:
        {
            struct GkWinObj *gkwin;
            memcpy(&gkwin,ParaAddr,sizeof(struct GkWinObj *));
            __GK_SetDirectScreen(gkwin);
            result = sizeof(struct GkWinObj *);
        } break;
        case CN_GKSC_SET_UNDIRECT_SCREEN:
        {
            struct GkWinObj *gkwin;
            memcpy(&gkwin,ParaAddr,sizeof(struct GkWinObj *));
            __GK_SetUnDirectScreen(gkwin);
            result = sizeof(struct GkWinObj *);
        } break;
        case CN_GKSC_SYNC_SHOW:
        {
            __gk_redraw_all();
        } break;
        case CN_GKSC_DSP_REFRESH:
        {
            struct DisplayObj *Display;
            memcpy(&Display,ParaAddr,sizeof(struct DisplayObj *));
            __gk_RefreshDisplay(Display);
            result = sizeof(struct DisplayObj *);
        } break;
        default: break;
    }       //for switch(command)
    return result;
}

//ptu32_t __GK_UsercallServer(void)
//{
//    u16 buf[CN_USERCALL_MSG_SIZE/2];
//    u16 id,size;
//
//    while(1)
//    {
//        if(MsgQ_Receive(g_tGkChunnel.usercall_msgq,(u8*)buf,CN_USERCALL_MSG_SIZE,1000*mS))
//        {
//            id   =buf[0];
//            size =buf[1];
//
//            switch(id)
//            {
//                case CN_GKUC_REPAINT:
//                {
//                    struct GkucParaRepaint *repaint;
//
//                    if(size==sizeof(struct GkucParaRepaint))
//                    {
//                        repaint =(struct GkucParaRepaint*)&buf[2];
//                        //printk("usercall:CN_GKUC_REPAINT,gkwin=%08XH\r\n",repaint->gk_win);
//                    }
//                }
//                break;
//                ////
//
//                default:
//                {
//                    printf("unknow usercall id: %08XH\r\n",id);
//                }
//                break;
//                ////
//            }
//        }
//    }
//}

struct EventECB  *g_ptServerEvent,*g_ptGuiAppWaiting = NULL;
void AppContinue(void)
{
    Int_SaveAsynSignal();
    if(g_ptGuiAppWaiting != NULL)     //gui app �ȴ���
    {
        if(g_ptGuiAppWaiting->event_status & CN_STS_SYNC_TIMEOUT)
            __DJY_ResumeDelay(g_ptGuiAppWaiting);    //����¼��ڳ�ʱ�ȴ������У�ȡ��
        g_ptGuiAppWaiting->event_status = CN_STS_EVENT_READY;
        g_ptGuiAppWaiting->wakeup_from = CN_STS_WAIT_GKDRAW;
        __DJY_RestorePrio( g_ptEventRunning );
        __DJY_EventReady(g_ptGuiAppWaiting);
        g_ptGuiAppWaiting = NULL;
    }
    Int_RestoreAsynSignal();
}

bool_t WaitGkDraw(u32 timeout)
{
    Int_SaveAsynSignal();
    __DJY_AddRunningToBlock(&g_ptGuiAppWaiting,CN_BLOCK_PRIO,timeout,CN_STS_WAIT_GKDRAW);

    //���濴���Ƿ�Ҫ�����ȼ��̳�
    __DJY_FollowUpPrio(g_ptServerEvent);
    Int_RestoreAsynSignal();  //�ָ��жϣ��������������л�
    //�������ﷵ�أ��ǳ�ʱ����ͬ���¼���ɡ�
    if(g_ptEventRunning->wakeup_from & CN_STS_SYNC_TIMEOUT)
    {//˵��ͬ������δ�����ӳ�ʱ����
        __DJY_RestorePrio( g_ptServerEvent );
        return false;
    }else
    {//˵���ǵõ�����������
        return true;
    }
}


//----gui kernel������---------------------------------------------------------
//����: �ӻ�����ȡ����ʾ���󣬲�����֮��
//����: ��
//����: ��
//-----------------------------------------------------------------------------
ptu32_t __GK_Server(void)
{
    u16 command;
    u16 len;
    u32 num,offset,size;
    g_ptServerEvent = g_ptEventRunning;

//    Lock_MutexPend(g_ptGkServerSync,CN_TIMEOUT_FOREVER);
//  DJY_SetEventPrio(249);
    while(1)
    {
//      printf("> round gkernel\r\n");
        //һ�ζ�ȡȫ�������Ϊ����ʱ�л��������������Թܵ��е����ݿ϶���������
        //��������ڰ�������Ŀ��ܡ�
        num = Ring_Read(&g_tGkChunnel.ring_syscall,(u8*)draw_chunnel_buf,
                                        CFG_GKERNEL_CMD_DEEP);
        if(num == 0)
        {
            AppContinue();
//          Lock_MutexPost(g_ptGkServerSync);
//          Lock_MutexPend(g_ptGkServerSync,CN_TIMEOUT_FOREVER);
//          Lock_SempPost(g_ptGkServerSync);
//          __DJY_RestorePrio( );
            Lock_SempPend(g_ptSyscallSemp,CN_TIMEOUT_FOREVER);
            continue;
        }
//      if(Lock_SempQueryFree(g_tGkChunnel.syscall_semp) == 0)
//          Lock_SempPost(g_tGkChunnel.syscall_semp);    //�ͷŹܵ�
        offset = 0;
        while(num > offset)
        {
            //���ڹܵ��е����ݿ��ܲ����룬�ʱ��������copy����������ֱ����ָ��
            //ָ���ǿ������ת��
            command = draw_chunnel_buf[offset] + ((u16)draw_chunnel_buf[offset+1]<<8);
            len =   draw_chunnel_buf[offset+2] + ((u16)draw_chunnel_buf[offset+3]<<8);
            offset += 4;
//            sync_draw = draw_chunnel_buf[offset];
//            offset += 1;
            size= __ExecOneCommand(command,(u8 *)draw_chunnel_buf + offset);
            if(len != size)
                debug_printf("gui kernel","����ȳ���\n\r");
            offset +=len;
        }   //for while(num != offset)
    }   //for while(1)

}

