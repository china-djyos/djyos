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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

//所属模块: gui
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: gui kernel主体部分
//其他说明:
//修订历史:
//2. 日期: 2011-07-18
//   作者:  罗侍田.
//   新版本号: V1.1.0
//   修改说明: 窗口数据结构实体已经改为由上层提供，但本文件一直没改，修改之。
//             增加__gk_vmalloc等4个函数，使在显存分配无特殊要求时，driver可以不
//             提供分配内存的函数。
//1. 日期: 2009-11-21
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
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

struct GkWinObj   *g_ptFocusWin;//显示焦点窗口,所有的显示都是对这个窗口的操作
static struct Object *s_ptDisplayDir;
static struct Object *s_ptWindowDir;
u32 g_u32DefaultBackColor,g_u32DefaultColor;
u16 g_u16GkServerEvtt,g_u16GkUsercallServerEvtt;
u16 g_u16GkServerEvent,g_u16GkUsercallServerEvent;


struct GkChunnel g_tGkChunnel;

extern struct MemCellPool *g_ptClipRectPool; //from gk_clip.c

//剪切域数量
#define CN_CLIP_INIT_NUM    100
struct ClipRect g_tClipRect[CN_CLIP_INIT_NUM];
struct GkWinObj *g_ptZ_Topmost;

//struct SemaphoreLCB *g_ptUsercallSemp;
//如果调用方希望gui kernel服务完成再返回，使用这个信号量
struct SemaphoreLCB *g_ptSyscallSemp;
//struct MutexLCB *g_ptGkServerSync;

u32 __ExecOneCommand(u16 DrawCommand,u8 *ParaAddr);

//gkwin在资源树种的位置与z序的联系:
// 1、在资源树中，所有子窗口按z序排列，最前端的是父窗口指向的第一个结点。
// 2、z_prio成员影响win在其兄弟结点中的位置，越小越前端
// 3、z_prio一般设为0，<=0均遮盖父窗口，若>0则被父窗口遮盖

static u8 draw_chunnel_buf[CFG_GKERNEL_CMD_DEEP];   //把环形缓冲区中的数据一次全部读出
static u8 write_chunnel_buf[CFG_GKERNEL_CMD_DEEP];  //用于和上层交互的环形缓冲区
//----初始化gui模块---------------------------------------------------------
//功能: 初始化gui模块，在资源队列中增加必要的资源结点
//参数: 模块初始化函数，没有参数
//返回: true = 成功
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

//根据像素格式，计算显示缓冲所需要的内存尺寸。
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

//----分配窗口缓存-------------------------------------------------------------
//功能: 分配窗口缓存，包括：1、窗口所需的bitmap,2、如果该窗口有rop4属性，还要分
//      配bitmsk_buf，3、pattern buffer。
//      虽然本函数可能需要分配三部分，但实际上是合并一起从堆中申请，然后再手工
//      分割成几个缓冲区的。因各缓冲区都是byte型指针，故无须担心对齐问题。
//参数: buf，用于返回结果的结构指针
//      xsize、ysize，显示区域尺寸
//      rop_code，窗口的rop_code代码，影响是否分配buf中的rop4_msk成员。
//返回: true=成功，false=失败
//注: 2416的2D加速不支持rop4，无须判断rop_code，只须分配pat和vbuf即可
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
    //计算显示缓冲所需要的内存尺寸。
    buf_size = __gk_cal_buf_size(xsize,ysize,&linesize,PixelFormat);
    buf_size = align_up_sys(buf_size);
    //同时分配bitmap数据结构、图片位图
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


//----重新分配窗口缓存---------------------------------------------------------
//功能: 当窗口尺寸改变，调用本函数重新分配缓存。逻辑上，本函数的功能="vfree-
//      重新vmalloc"的组合，新设置一个函数，是为了提高效率。因为内存是按块分配
//      的，内存分配是相当耗时的工作，vmalloc分配的窗口缓存，可能比实际的大，当
//      窗口尺寸调整不大的话，可能无需重新分配。而用户通过拖动来改变窗口尺寸时，
//      实际上是一点一点改变的，每次都重新分配的话，系统开销很大，且易产生碎片。
//      vrmalloc函数先检查buf中的缓冲区是否仍然满足需求，若满足，则不重新分配。
//      但可能会跟据xsize和ysize调整指针。
//参数: buf，原窗口缓存指针，也用于返回结果
//      xsize、ysize，显示区域尺寸
//      rop_code，窗口的rop_code代码
//返回: true=成功，false=失败
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

    //重新分配显存，若原显存仍然符合要求，可能不会重新分配。
    //若需重新分配，必须采取先分配后释放的办法，因为djyos是多线程环境，先释放后
    //申请的话，虽然能少许增加分配的成功率，但释放后可能因抢占，内存被别的高优先
    //级事件抢去的话，就会造成分配失败。先分配后释放，如果分配失败，至少还可以
    //确保原来的显存不丢失。
    if(PixelFormat == CN_SYS_PF_DISPLAY)
        PixelFormat = disp->pixel_format;
    //计算显示缓冲所需要的内存尺寸。
    buf_size = __gk_cal_buf_size(xsize,ysize,&linesize,PixelFormat);
    buf_size = align_up_sys(buf_size);
    newsize = buf_size + sizeof(struct RectBitmap);

    oldsize = M_CheckSize(gkwin->wm_bitmap); //查询原指针实际分配的内存
    if(newsize <= oldsize)
    {
        //新的显存需求在原实际尺寸相差在1/8以内的或者小于最小内存分配尺寸的，
        //不重新分配显存
        if((oldsize - newsize) <= (oldsize>>3))
            remalloc = false;       //无需重新分配
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
            result = false;   //新分配内存失败，保持原来不变，不释放原内存。
        }
    }else                     //无须重新分配
    {
        result = true;
    }
    return result;
}


//----释放窗口缓存-------------------------------------------------------------
//功能: 释放一个窗口的缓存。
//参数: disp，窗口所属显示器
//      buf，用于返回结果的结构指针
//返回: 无
//-----------------------------------------------------------------------------
void __gk_vfree(struct DisplayObj *disp,struct GkWinObj *gkwin)
{
    //分配内存时，显存和bitmap结构是同时分配的，故只须调用
    //m_free一次，两者均释放
    M_FreeHeap(gkwin->wm_bitmap,disp->DisplayHeap);
}

//----取z轴片段的起始窗口(窗口及其子孙窗口)------------------------------------
//功能: 每一个窗口及其子孙窗口，都在z轴中占据连续的一段，本函数取该段的起始窗口(
//      最后端的窗口)。
//参数: gkwin，窗口指针
//返回: 起始窗口指针
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
        if(temp == NULL)        //result已经没有子窗口了
            break;
        temp = OBJ_GetPrev(temp);
        current = (struct GkWinObj*)OBJ_GetPrivate(temp);
        if(current->WinProperty.Zprio <= CN_ZPRIO_DEFAULT )
            break;      //previous是所有子窗口中最后端的窗口，它result前端，代表
                        //所有子窗口均在result前端
        else
        {
            result = current;
        }
    }
    return result;
}

//----取z轴片段的结束窗口(窗口及其子窗口)--------------------------------------
//功能: 每一个窗口及其子孙窗口，都在z轴中占据连续的一段，本函数取该段的结束窗口(
//      最前端的窗口)。
//参数: gkwin，窗口指针
//返回: 结束窗口指针
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
        if(temp == NULL)        //result已经没有子窗口了
            break;
        current = (struct GkWinObj*)OBJ_GetPrivate(temp);
        if(current->WinProperty.Zprio > CN_ZPRIO_DEFAULT )
            break;      //第一个子窗口是所有子窗口中最前端的窗口，它在result后端
                        //代表所有子窗口均在result后端
        else
        {
            result = current;
        }
    }
    return result;
}


//----创建桌面-----------------------------------------------------------------
//功能: 桌面是所有显示器的第一个窗口；所有的显示器，必须先创建桌面才能创建窗口，
//      桌面的尺寸不能小于显示器的尺寸，但可以大于显示器尺寸。桌面在刷新screen
//      的过程中和普通窗口是等同的。
//参数: para，参数数据结构，其成员含义见结构定义处。
//返回: 新窗口句柄(指针)
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
        desktop_x = para->width;            //桌面可以大于显示器尺寸
    else
        desktop_x = display->width;   //桌面不能小于显示器尺寸
    if(para->height >= display->height)
        desktop_y = para->height;
    else
        desktop_y = display->height;
    if(display->frame_buffer == NULL)
    {
        if(para->buf_mode == CN_WINBUF_BUF)
        {
            return NULL;        //无帧缓冲的显示器，不允许创建缓冲桌面
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
    desktop = para->desktop;    //para->desktop由调用者提供内存，传指针过来
    //下面初始化桌面，桌面也是按窗口管理的
    desktop->z_back = desktop;
    desktop->z_top = desktop;
    strncpy(desktop->win_name,para->name,CN_GKWIN_NAME_LIMIT);
    desktop->win_name[CN_GKWIN_NAME_LIMIT] = '\0';
    desktop->redraw_clip = NULL;
    desktop->changed_clip = NULL;
    desktop->disp = display;            //所属显示器
    desktop->WinProperty.Zprio = CN_ZPRIO_DEFAULT;                //桌面的优先级
    desktop->WinProperty.DestBlend = CN_GKWIN_DEST_HIDE;        //本参数对桌面无效
    desktop->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_NONE; //清屏后才显示
    desktop->WinProperty.DirectDraw = CN_GKWIN_UNDIRECT_DRAW; //非直接写屏
    desktop->WinProperty.BoundLimit = CN_BOUND_LIMIT;
    desktop->WinProperty.Visible = CN_GKWIN_VISIBLE;
    desktop->RopCode = (struct RopGroup){ 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  }; //桌面固定
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
    if(BufM)      //缓冲模式，为桌面准备缓冲区
    {
        if( __gk_vmalloc(display,desktop,para->PixelFormat) )//分配显存
        {
            msk_size = ((desktop_x +7)/8+7)/8*((desktop_y +7)/8);
            //存在frame buffer时才需要changed_msk
            desktop->changed_msk.bm_bits = M_MallocHeap(msk_size,
                                            display->DisplayHeap,0);
            if( desktop->changed_msk.bm_bits == NULL)   //分配changed_msk内存失败
            {
                display->z_topmost = NULL;
                display->desktop = NULL;
                M_FreeHeap(desktop->changed_msk.bm_bits,display->DisplayHeap);
                Mb_Free(g_ptClipRectPool,clip);
                OBJ_Delete(NewWindow);
                __gk_vfree(display,desktop);
                DJY_SaveLastError(EN_GK_NO_MEMORY);
                debug_printf("gkwin","显存不足\n\r");
                return NULL;
            }
            else
            {//分配changed_msk内存成功，将显存再分配
                memset(desktop->changed_msk.bm_bits,0,msk_size);
                desktop->changed_msk.PixelFormat = CN_SYS_PF_GRAY1;
                desktop->changed_msk.height = desktop_y/8;
                desktop->changed_msk.width = desktop_x/8;
                desktop->changed_msk.linebytes = ((desktop_x+7)/8+7)/8;
                desktop->HyalineColor = 0;

                desktop->wm_bitmap->ExColor = para->BaseColor;
            }
        }
        else               //分配显存失败，
        {
            Mb_Free(g_ptClipRectPool,clip);
            OBJ_Delete(NewWindow);
            display->z_topmost = NULL;
            display->desktop = NULL;
            DJY_SaveLastError(EN_GK_NO_MEMORY);
            debug_printf("gkwin","显存不足\n\r");
            return NULL;
        }

    }
    else                   //非缓冲模式，窗口没有缓冲区
    {
        desktop->wm_bitmap = NULL;
        desktop->changed_msk.bm_bits = NULL; //没有这一句会有不敢想象的bug
    }
    //桌面可视域，可视域大小与显示器尺寸大小一致
    clip->next = clip;
    clip->previous = clip;
    clip->rect.left = 0;
    clip->rect.top = 0;
    clip->rect.right = display->width;
    clip->rect.bottom = display->height;
    desktop->visible_clip = clip;
    desktop->visible_bak = NULL;
    display->reset_clip = true;

    //用给定的颜色填充桌面
    para_fill.gkwin = desktop;
    para_fill.color = para->color;
    __GK_FillWin(&para_fill);
    //返回新窗口指针
    return desktop;
}


//----创建窗口-----------------------------------------------------------------
//功能: 创建一个窗口，新窗口的优先级为0，位于所有优先级=0的窗口的最前端。
//参数: para，参数数据结构，其成员含义见结构定义处。
//返回: 新窗口句柄(指针)
//-----------------------------------------------------------------------------
struct GkWinObj *__GK_CreateWin(struct GkscParaCreateGkwin *para)
{
    u32 size;
    struct RopGroup RopCode;
    s32 width,height;
    bool_t BufM;
    struct GkWinObj *target_section;
    struct GkWinObj *parent;                       //gkwin的父窗口
    struct Object *NewWindow;
    struct DisplayObj *display;
    //z轴中被移动的窗口段最后端的窗口
    struct GkWinObj *move_start;
    //z轴中被移动的窗口段最前端的窗口
    struct GkWinObj *move_end;
    struct GkWinObj *gkwin;
    struct GkscParaFillWin para_fill;

    width = para->right - para->left;
    height = para->bottom - para->top;
    display = para->parent_gkwin->disp;
//  gkwin = para->gkwin;    //para->gkwin由调用者提供内存，传指针过来
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
            return NULL;        //父窗口无缓冲，不允许子窗口有缓冲。
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
//      return NULL;   //窗口属性不支持rop2和需要Dst Alpha参与的alpha混合
    if((RopCode.AlphaMode & CN_ALPHA_MODE_NEED_AD) != 0)
        return NULL;   //窗口属性不支持需要Dst Alpha参与的alpha混合

    *gkwin = *parent;               //新窗口拷贝父窗口的属性

    strncpy(gkwin->win_name,para->name,CN_GKWIN_NAME_LIMIT);
    gkwin->win_name[CN_GKWIN_NAME_LIMIT] = '\0';

    //给定的新窗口的坐标是相对其父窗口的
    gkwin->left = para->left;
    gkwin->top = para->top;
    gkwin->right = para->right;
    gkwin->bottom = para->bottom;
    //窗口的绝对位置，相对于所属screen的桌面原点
    gkwin->absx0 = para->left + para->parent_gkwin->absx0;
    gkwin->absy0 = para->top + para->parent_gkwin->absy0;
    gkwin->WinProperty.Zprio = CN_ZPRIO_DEFAULT;                //新建窗口默认为0
    gkwin->WinProperty.DestBlend = __GK_RopIsNeedDestination(RopCode);
    gkwin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_NONE; //清屏后才显示
    gkwin->WinProperty.DirectDraw = CN_GKWIN_UNDIRECT_DRAW; //非直接写屏
    gkwin->WinProperty.BoundLimit = CN_BOUND_LIMIT;
    gkwin->WinProperty.Visible = CN_GKWIN_VISIBLE;

    gkwin->RopCode = RopCode;

    if(BufM)              //缓冲模式，为窗口准备缓冲区
    {
        if( __gk_vmalloc(display,gkwin,para->PixelFormat) )  //分配显存
        {
            size = (((width +7)/8+7)/8 *((height +7)/8));
            //存在frame buffer时才需要changed_msk
            gkwin->changed_msk.bm_bits = M_MallocHeap(size,
                                                    display->DisplayHeap,0);
            if(gkwin->changed_msk.bm_bits == NULL)   //分配修改掩码内存失败
            {
                __gk_vfree(display,gkwin);
                DJY_SaveLastError(EN_GK_NO_MEMORY);
                debug_printf("gkwin","显存不足\n\r");
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
        else               //分配显存失败，
        {
            DJY_SaveLastError(EN_GK_NO_MEMORY);
            debug_printf("gkwin","显存不足\n\r");
            return NULL;
        }
    }
    else                   //非缓冲模式，窗口没有缓冲区
    {
        gkwin->wm_bitmap = NULL;
        gkwin->changed_msk.bm_bits = NULL;//没有这一句会有不敢想象的bug
    }

    //以下把新窗口连接到资源队列中，并插入到z轴中
    if(OBJ_GetChild(para->parent_gkwin->HostObj) == NULL)  //父窗口无子窗口
    {
        NewWindow = OBJ_NewChild(parent->HostObj, (fnObjOps)-1, (ptu32_t)gkwin,
                                    (const char*)(gkwin->win_name));
        if(NewWindow == NULL)
            return NULL;
        gkwin->HostObj = NewWindow;
        //以下4句在z轴中把win插入父节点的前端
        gkwin->z_back = parent;
        gkwin->z_top = parent->z_top;
        parent->z_top->z_back = gkwin;
        parent->z_top = gkwin;
        if(display->z_topmost == parent)
            display->z_topmost = gkwin;
    }
    else
    {//父窗口有子窗口
        //取z轴中被移动的与gkwin同级的窗口段开始和结束窗口段
        move_end = (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetChild(parent->HostObj));
        move_start = (struct GkWinObj*)OBJ_GetPrivate(OBJ_GetPrev(move_end->HostObj));
        target_section = move_end;
        while(1)
        {//扫描同级窗口中和目标窗口优先级相同的窗口，扫描以z轴为对象
            //扫描顺序:从z轴段前端向后端进行扫描，只扫兄弟窗口，不扫子窗口
            if(target_section->WinProperty.Zprio >= CN_ZPRIO_DEFAULT)
                break;
            //扫描目标窗口所在z轴段最后一窗口，终止跳出
            if(target_section == move_start)
                break;
            target_section = (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetNext(target_section->HostObj));
        }
        if(target_section->WinProperty.Zprio == CN_ZPRIO_DEFAULT)
        {//同级窗口中存在和gkwin的prio相等的窗口
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
            //获取target_section和其子窗口所在z轴段的结束窗口(最前端)
            //新窗口插入到它的前端
            if(move_end == display->z_topmost)
                display->z_topmost = gkwin;
            //插到目标窗口前端

            gkwin->z_back = move_end;
            gkwin->z_top = move_end->z_top;
            move_end->z_top->z_back = gkwin;
            move_end->z_top = gkwin;
        }
        else //兄弟窗口中没有优先级=0的窗口
        {
            //同级窗口中有优先级大于0的，资源队列中新窗口插入其前端
            if(target_section->WinProperty.Zprio > CN_ZPRIO_DEFAULT)
            {
//                NewWindow = OBJ_AddToPrevious(target_section->HostObj, NULL, (ptu32_t)gkwin,
//                                                (const char *)(gkwin->win_name));
                NewWindow = OBJ_NewPrev(target_section->HostObj, (fnObjOps)-1, (ptu32_t)gkwin,
                                         (const char *)(gkwin->win_name));
                if(NewWindow == NULL)
                    return NULL;
            }
            else    //同级窗口优先级均高于新窗口(prio = 0)
            {
                //新窗口在资源队列中处于同级窗口最后端
//                NewWindow = OBJ_AddToNext(target_section->HostObj, NULL, (ptu32_t )gkwin,
//                                            (const char*)(gkwin->win_name));
                NewWindow = OBJ_NewNext(target_section->HostObj, (fnObjOps)-1, (ptu32_t )gkwin,
                                            (const char*)(gkwin->win_name));
                if(NewWindow == NULL)
                    return NULL;
            }
            gkwin->HostObj = NewWindow;
            //新创建的窗口gkwin优先级为0，无论同级窗口优先级均如何分布，新
            //窗口肯定在父窗口前端
            if(parent == display->z_topmost)
                display->z_topmost = gkwin;
            //插到目标窗口前端
            gkwin->z_back = parent;
            gkwin->z_top = parent->z_top;
            parent->z_top->z_back = gkwin;
            parent->z_top = gkwin;
        }
    }
    //设置窗口的可见边界，不受窗口互相遮挡影响，必须设置资源队列后才能调用
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

//----改变窗口尺寸和位置-------------------------------------------------------
//功能: 改变一个窗口的尺寸以及位置，如果没改尺寸，实际上就是一个gk_move_win。
//参数: gcwin，目标窗口
//      new_area，新的窗口尺寸和位置
//      mode，改尺寸后，窗口图像处理模式，0=保持不变，1=拉伸或压缩，
//            缓冲窗口才有效
//返回: true = 成功，false = 失败(一般是内存分配导致)
//注: 有缓冲区的情况下，如果只是移动了位置，不需要重新绘制。
//    如果修改了尺寸，应用程序应该重绘，直接重绘即可，无须等待重绘消息。
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
            return false;                   //桌面尺寸不能小于显示器尺寸
    }
    if((right < left) || (bottom < top))    //尺寸数据不合法
        return false;

    if(((right-left) == (cwawin->right-cwawin->left))
            && ((bottom-top) == (cwawin->bottom - cwawin->top)))
    {
        movwin_para.gkwin = cwawin;
        movwin_para.left = left;
        movwin_para.top = top;
        __GK_MoveWin(&movwin_para);        //只是移动了窗口，尺寸未变
        return true;
    }
    if(cwawin->wm_bitmap != NULL)
    {
        bak = *cwawin;
        if( !__gk_vrmalloc(disp,&bak))
        {
            DJY_SaveLastError(EN_GK_NO_MEMORY);
            debug_printf("gkwin","显存不足\n\r");
            return false;   //分配显存失败，窗口尺寸未改变(仍然有效)
        }else
        {
            if(bak.wm_bitmap != cwawin->wm_bitmap)
                M_FreeHeap(cwawin->wm_bitmap,disp->DisplayHeap);
            *cwawin = bak;
        }
    }
    if(cwawin == disp->desktop)
    {   //目标窗口是桌面，有额外限制:桌面必须覆盖显示器区域
        if(left > 0)    //桌面左边界必须覆盖显示器
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
            cwawin->limit_left = -left;//桌面的可显示左边界应该刚好在显示器的左边界
        }
        if(top > 0)    //桌面上边界必须覆盖显示器
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
            cwawin->limit_top = -top;//桌面的可显示上边界应该刚好在显示器的上边界
        }

        if(right < disp->width)     //桌面右边界必须覆盖显示器
        {
            cwawin->right = disp->width;
        }
        else
        {
            cwawin->right = right;
        }
        //桌面的可显示右边界应该刚好在显示器的右边界
        cwawin->limit_right = cwawin->limit_left + disp->width;

        if(bottom < disp->height)     //桌面右边界必须覆盖显示器
        {
            cwawin->bottom = disp->height;
        }
        else
        {
            cwawin->bottom = bottom;
        }
        //桌面的可显示下边界应该刚好在显示器的下边界
        cwawin->limit_bottom = cwawin->limit_top + disp->height;
    }else
    {
        delta_left = para->left-cwawin->left;
        delta_top = para->top-cwawin->top;
        cwawin->absx0 += delta_left;//修改窗口绝对坐标
        cwawin->absy0 += delta_top;

        cwawin->left = left;                      //修改窗口相对坐标
        cwawin->top = top;
        cwawin->right = right;
        cwawin->bottom = bottom;
        __GK_SetBound(cwawin);                        //设置可显示边界
    }

    cwawin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_ALL;

    changing = cwawin->HostObj;
    current = OBJ_ForeachScion(changing, changing);
    //遍历gkwin的所有子孙窗口,遍历完成后，current就=NULL了
    while(current != NULL)
    {
        temp = (struct GkWinObj*)OBJ_GetPrivate(current);
        temp->absx0 += delta_left;        //修改窗口绝对坐标
        temp->absy0 += delta_top;
        current = OBJ_ForeachScion(changing, current);
    }
    current = OBJ_ForeachScion(changing, changing);
    //必须分两次遍历，才能确保Set 孙窗口时，子窗口已经改好了absx0
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

//----过继窗口-----------------------------------------------------------------
//功能：把一个窗口连同其子孙窗口从其父窗口中移出，变成其他窗口的子窗口。窗口的
//      其他属性不变。在新位置的Z序序列中，排在优先级相同的窗口的最前端。
//参数：gkwin，目标窗口
//      NewParent，新的父窗口
//返回：无
//-----------------------------------------------------------------------------
void __GK_AdoptWin(struct GkscParaAdoptWin *para)
{
    struct GkWinObj *last,*foremost,*gkwin,*Ztarget,*NowWin;
    struct Object *parent,*point;
    struct DisplayObj *display;
    gkwin = para->gkwin;
    parent = para->NewParent->HostObj;
    display = gkwin->disp;
    if(OBJ_GetParent(gkwin->HostObj) == parent)  //新的父窗口没改变
        return ;

    //因gkwin可能有子窗口，其与子窗口一起，在Z轴中占据连续的一段，
    //取gkwin在Z轴中最前和最后端的窗口
    last = __GK_GetZsectionStart(gkwin);
    foremost = __GK_GetZsectionEnd(gkwin);

    //以下过程处理对象队列。
    if(OBJ_GetChild(parent) == NULL)
    {
        OBJ_InsertToChild(parent,gkwin->HostObj);
        NowWin = (struct GkWinObj *)OBJ_GetPrivate(parent);
        if(gkwin->WinProperty.Zprio > 0)
            Ztarget = NowWin->z_back;
        else
            Ztarget = NowWin;
    }
    else    //新的窗口有子窗口，要查找新窗口的插入点
    {
        //找一个Z优先级不高于新窗口的窗口
        point = OBJ_ForeachChild(parent,parent);
        while( point != NULL )
        {
            NowWin = (struct GkWinObj *)OBJ_GetPrivate(point);
            if(NowWin->WinProperty.Zprio >= gkwin->WinProperty.Zprio)
                break;
            point = OBJ_ForeachChild(parent,point);
        }
        if(point == NULL)   //没有找到Z优先级低于gkwin的窗口，因是循环链表，最后
                            //端窗口的后面，也就是最前端窗口的前面
        {
            point = OBJ_GetChild(parent);
        }
        OBJ_InsertToPrev(point, gkwin->HostObj); //移到对象队列中适当位置

        //看gkwin是否会成为父窗口的头结点
        if((gkwin->WinProperty.Zprio <= 0) && (point == OBJ_GetChild(parent)))
        {
            OBJ_ChildMoveToOrev(parent);
        }

        //以下查找Z序队列的插入点，插入点在point所在窗口段的前端，但要判断是否
        //跨越parent
        NowWin = (struct GkWinObj *)OBJ_GetPrivate(point);
        if( (NowWin->WinProperty.Zprio > 0) && (gkwin->WinProperty.Zprio <= 0) )
            Ztarget = (struct GkWinObj *)parent;       //父窗口在插入点和gkwin之间
        else
            Ztarget = __GK_GetZsectionEnd( NowWin );
    }
    foremost->z_top->z_back = last->z_back;
    last->z_back->z_top = foremost->z_top;
    //当 Ztarget->z_top==foremos==lastt时这样操作会出错
    //改为先移除在插入
    Ztarget->z_top->z_back = foremost;
    foremost->z_top = Ztarget->z_top;
    Ztarget->z_top = last;
    last->z_back = Ztarget;
    __GK_ScanNewVisibleClip(display);
    display->reset_clip = true;
}

//----移动窗口-----------------------------------------------------------------
//功能: 改变窗口在父窗口内的相对位置，由于子窗口的坐标是相对于父窗口的，故移动窗
//      口时，连子窗口一起移动。本函数是gk_change_win_area函数的一个特例。
//参数: gcwin，目标窗口
//      left、top，新的左上角坐标，相对于父窗口
//返回: 无
//-----------------------------------------------------------------------------
void __GK_MoveWin(struct GkscParaMoveWin *para)
{
    s32 delta_top,delta_left;
    struct GkWinObj *movewin;
    struct Object *moving,*current;
    struct DisplayObj *disp;
    movewin = para->gkwin;
    if((para->left == movewin->left) && (para->top == movewin->top))
        return;                                 //位置没有修改，直接返回。
    disp = movewin->disp;
    if(movewin == disp->desktop)
    {   //目标窗口是桌面，有额外限制:桌面必须覆盖显示器区域
        if(para->left > 0)    //桌面左边界必须覆盖显示器
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
            movewin->limit_left = -para->left;//桌面的可显示左边界应该刚好在显示器的左边界
        }
        if(para->top > 0)    //桌面上边界必须覆盖显示器
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
            movewin->limit_top = -para->top;//桌面的可显示上边界应该刚好在显示器的上边界
        }

        movewin->right += delta_left;
        if(movewin->right < disp->width)     //桌面右边界必须覆盖显示器
        {
            movewin->right = disp->width;
        }
        //桌面的可显示右边界应该刚好在显示器的右边界
        movewin->limit_right = movewin->limit_left + disp->width;

        movewin->bottom += delta_top;
        if(movewin->bottom < disp->height)     //桌面右边界必须覆盖显示器
        {
            movewin->bottom = disp->height;
        }
        //桌面的可显示下边界应该刚好在显示器的下边界
        movewin->limit_bottom = movewin->limit_top + disp->height;
    }
    else
    {
        delta_left = para->left-movewin->left;
        delta_top = para->top-movewin->top;
        movewin->absx0 += delta_left;//修改窗口绝对坐标
        movewin->absy0 += delta_top;

        movewin->left = para->left;                      //修改窗口相对坐标
        movewin->top = para->top;
        movewin->right += delta_left;
        movewin->bottom += delta_top;
//      if((movewin->absx0 <0) || (movewin->absy0 <0))
//          delta_left = 0;
        __GK_SetBound(movewin);                        //设置可显示边界
    }
    moving = movewin->HostObj;
    current = OBJ_ForeachScion(moving, moving);
    //遍历gkwin的所有子孙窗口
    while(current != NULL)
    {
        movewin = (struct GkWinObj*)OBJ_GetPrivate(current);
        movewin->absx0 += delta_left;        //修改窗口绝对坐标
        movewin->absy0 += delta_top;
        current = OBJ_ForeachScion(moving, current);
    }
    current = OBJ_ForeachScion(moving, moving);
    //必须分两次遍历，才能确保Set 孙窗口时，子窗口已经改好了absx0
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

//----设置可显示边界-----------------------------------------------------------
//功能: 任何窗口，如果即使不考虑被z轴前端窗口剪切，其窗口可显示范围也是有限的，
//      受bound_limit=true的、世代最近的祖先窗口的可显示边界限制。窗口位置改变
//      后，需要重置可显示边界。
//      gkwin，被重置的窗口
//返回: 无
//-----------------------------------------------------------------------------
void __GK_SetBound(struct GkWinObj *gkwin)
{
    struct GkWinObj *ancestor;
    s32 gkwin_absx,gkwin_absy,ancestor_absx,ancestor_absy;
    ancestor = gkwin;
    //获取祖先窗口中第一个边界受父窗口限制的第一个窗口，gkwin的边界受该窗口限定
    //桌面窗口一定是个边界首先窗口
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

    gkwin_absx = gkwin->absx0;  //取本窗口的绝对坐标，也即gkwin->left的绝对坐标
    gkwin_absy = gkwin->absy0;
    //取限制本窗口边界的窗口的可见边界绝对坐标
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
//----设置边界模式-------------------------------------------------------------
//功能: 设定窗口的显示边界是否受父窗口限制，限制后，子窗口超出父窗口的部分将不予
//      显示，desktop的直接子窗口默认受限，不能更改。
//参数: gkwin，目标窗口
//      mode，true为受限，false为不受限
//返回: 无
//-----------------------------------------------------------------------------
void __GK_SetBoundMode(struct GkscParaSetBoundMode *para)
{
    struct GkWinObj *current;
    struct Object *Scion,*ancestor;
    if(para->gkwin == NULL)
        return;
    if(para->gkwin == para->gkwin->disp->desktop)   //桌面不可修改边界模式
        return;
    if(para->gkwin->WinProperty.BoundLimit == para->mode)      //模式未改变
        return;
    if(para->gkwin->disp->desktop->HostObj == OBJ_GetParent(para->gkwin->HostObj))
        return;                         //直接放在桌面的子窗口，不许改变
    //目标窗口边界模式改变，窗口属性改变
    para->gkwin->WinProperty.BoundLimit = CN_BOUND_LIMIT;
    __GK_SetBound(para->gkwin);
    ancestor = para->gkwin->HostObj;
    Scion = OBJ_ForeachScion(ancestor,ancestor);
    //遍历gkwin的所有子孙窗口
    while( Scion != NULL)
    {
        current = (struct GkWinObj*)OBJ_GetPrivate(Scion);
        __GK_SetBound(current);
    }
    __GK_ScanNewVisibleClip(para->gkwin->disp);
    para->gkwin->disp->reset_clip = true;
}

//----设置窗口是否可视---------------------------------------------------------
//功能：设置一个窗口的可视属性，桌面不能设置
//参数: gkwin，目标窗口
//      visible，CN_GKWIN_VISIBLE=可视，CN_GKWIN_HIDE=隐藏
//返回：无
//-----------------------------------------------------------------------------
void __GK_SetVisible(struct GkscParaSetVisible *para)
{
    struct GkWinObj *gkwin;
    struct DisplayObj *display;

    gkwin = para->gkwin;
    display = para->gkwin->disp;
    if(gkwin == display->desktop)  //桌面窗口可视属性不可改变
        return;
    gkwin->WinProperty.Visible = para->Visible;

    __GK_ScanNewVisibleClip(gkwin->disp);
    gkwin->disp->reset_clip = true;

}

//----设置窗口显示优先级-------------------------------------------------------
//功能: 设置一个窗口的优先级，优先级决定在同级窗口中的z轴相对位置，数字越小，在
//      z轴中越靠前，优先级<=0将覆盖父窗口，反之被父窗口覆盖。窗口改变优先级后，
//      它在Z轴中的顺序可能会改变，屏幕内容也可能会改变。
//      由于被改变优先级的窗口可能还有子窗口，所以，在z轴中被移动的，不是一个窗
//      口，而是连续的一段窗口。
//参数: gkwin，目标窗口
//      prio，新优先级
//返回: 无
//-----------------------------------------------------------------------------
void __GK_SetPrio(struct GkscParaSetPrio *para)
{
    struct GkWinObj *target_section,*gkwin;
    struct DisplayObj *display;
    struct GkWinObj *parent;       //gkwin的父窗口
    //z轴中被移动的窗口段最后端的窗口
    struct GkWinObj *section_start;
    //z轴中被移动的窗口段最前端的窗口
    struct GkWinObj *section_end,*target_win;

    gkwin = para->gkwin;
    display = para->gkwin->disp;
    if(gkwin == display->desktop)  //桌面窗口的优先级不可改变
        return;

    parent = (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetParent(gkwin->HostObj));
    //取z轴中被移动的窗口段最前端的窗口段
    section_end = (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetChild(parent->HostObj));
    section_start = (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetPrev(section_end->HostObj));
    target_section = section_end;
    while(1)
    {   //查找同级窗口中和优先级等于para->prio的的窗口，扫描以z轴为对象
        //扫描顺序:从z轴段前端向后端进行扫描，只扫兄弟窗口，不扫子窗口
        if(target_section->WinProperty.Zprio >= (s32)para->prio)
            break;
        //扫描目标窗口所在z轴段最后一窗口，终止跳出
        if(target_section == section_start)
            break;
        target_section = (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetNext(target_section->HostObj));
    }
    if(target_section->WinProperty.Zprio >= (s32)para->prio)
    {   // 找到gkwin同级窗口中优先级低于或等于新prio的窗口
        if((target_section == gkwin)
            || (target_section == (struct GkWinObj *)OBJ_GetPrivate(OBJ_GetNext(gkwin->HostObj))))
        {
            //在资源队列中无须移动，但优先级队列可能要移动
            if((gkwin->WinProperty.Zprio <= CN_ZPRIO_DEFAULT)
                    && (para->prio > CN_ZPRIO_DEFAULT))    //移到父窗口后端
            {
                //获取gkwin及其子孙窗口所在z轴段的开始和结束窗口
                section_start = __GK_GetZsectionStart(gkwin);
                section_end = __GK_GetZsectionEnd(gkwin);
                //考虑gkwin要插入的窗口是否是最前端窗口,
                //是,则将gkwin所在Z轴段最前端的窗口调整为整个Z轴的最前端窗口
                if(section_end == display->z_topmost)
                    display->z_topmost = parent;
                //将gkwin所在z轴段从z轴取出来
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
                            && (para->prio <= CN_ZPRIO_DEFAULT))//移到父窗口前端
            {
                //获取gkwin及其子孙窗口所在z轴段的开始和结束窗口
                section_start = __GK_GetZsectionStart(gkwin);
                section_end = __GK_GetZsectionEnd(gkwin);
                //考虑gkwin要插入的窗口是否是最前端窗口,
                //是,则将gkwin所在Z轴段最前端的窗口调整为整个Z轴的最前端窗口
                if(parent == display->z_topmost)
                    display->z_topmost = section_end;
                //将gkwin所在z轴段从z轴取出来
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
            else    //其他情况，无须移动z轴
            {
                gkwin->WinProperty.Zprio = para->prio;
            }
        }
        else    //资源队列须移动
        {
            OBJ_InsertToPrev(target_section->HostObj, gkwin->HostObj);

            if((para->prio <= CN_ZPRIO_DEFAULT)
                    && (target_section->WinProperty.Zprio > CN_ZPRIO_DEFAULT))
                target_win = parent;
            else
                target_win = __GK_GetZsectionEnd(target_section);
            //获取gkwin及其子孙窗口所在z轴段的开始和结束窗口
            section_start = __GK_GetZsectionStart(gkwin);
            section_end = __GK_GetZsectionEnd(gkwin);
            //考虑gkwin要插入的窗口是否是最前端窗口,
            //是,则将gkwin所在Z轴段最前端的窗口调整为整个Z轴的最前端窗口
            if(target_win == display->z_topmost)
                display->z_topmost = section_end;
            //将gkwin所在z轴段从z轴取出来
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
    {//同级窗口优先级均高于新prio
        //OBJ_MoveToNext(target_section->HostObj,gkwin->HostObj);
        OBJ_InsertToNext(target_section->HostObj,gkwin->HostObj);
        if((para->prio > CN_ZPRIO_DEFAULT)
                    && (target_section->WinProperty.Zprio <=CN_ZPRIO_DEFAULT))
        {
            target_win = parent;
        }
        else
        {
            //找到new_location及其子孙组成的z轴段中最后端的窗口
            target_win = __GK_GetZsectionStart(target_section);
        }
        //以下把gkwin取出并插入move_end_temp后端
        //获取gkwin及其子孙窗口所在z轴段的开始和结束窗口
        section_start = __GK_GetZsectionStart(gkwin);
        section_end = __GK_GetZsectionEnd(gkwin);

        //将gkwin所在z轴段从z轴取出来
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

//----设置光栅操作码---------------------------------------------------------
//功能: 改变窗口的光栅操作码。非缓冲窗口和桌面只支持CN_R2_COPYPEN，
//        不可修改光栅操作码。
//参数: para，参数
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t __GK_SetRopCode(struct GkscParaSetRopCode *para)
{
    struct GkWinObj *mygkwin;
    struct DisplayObj *mydisplay;

    mygkwin = para->gkwin;
    mydisplay = mygkwin->disp;
    //桌面和非缓冲窗口不可修改RopCode
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
//      mygkwin->RopCode &= ~CN_ROP_ROP2_MSK;   //窗口属性不支持rop2
        return true;
    }
}

//----设置窗口透明色-----------------------------------------------------------
//功能: 改变窗口透明色
//参数: gkwin，目标窗口
//      transparentcolor，设置的透明色,颜色格式是真彩色
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t __GK_SetHyalineColor(struct GkscParaSetHyalineColor *para)
{
    u32 color;
    //桌面不需要KeyColor
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
        //无须做任何事
    }
    else
    {
        para->gkwin->RopCode.HyalineEn = 1;
        para->gkwin->WinProperty.DestBlend = CN_GKWIN_DEST_VISIBLE;
        para->gkwin->WinProperty.ChangeFlag = CN_GKWIN_CHANGE_ALL;
    }
    return true;
}
//----设置窗口直接写屏属性-----------------------------------------------------
//功能: 设置窗口直接写屏属性。
//参数: gkwin，目标窗口
//返回: 无
//-----------------------------------------------------------------------------
void __GK_SetDirectScreen(struct GkWinObj *gkwin)
{
    gkwin->WinProperty.DirectDraw = CN_GKWIN_DIRECT_DRAW;
}
//----撤销窗口直接写屏属性-----------------------------------------------------
//功能: 撤销窗口直接写屏属性。
//参数: gkwin，目标窗口
//返回: 无
//-----------------------------------------------------------------------------
void __GK_SetUnDirectScreen(struct GkWinObj *gkwin)
{
    gkwin->WinProperty.DirectDraw = CN_GKWIN_UNDIRECT_DRAW;
}
//----销毁窗口(子窗口)---------------------------------------------------------
//功能: 释放窗口所占用的资源。
//参数: gkwin，目标窗口
//返回: 无
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
        if(gkwin->wm_bitmap != NULL)//有帧缓冲在缓冲模式下
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
//----销毁窗口-------------------------------------------------------------
//功能: 释放窗口所占用的资源。销毁窗口，同时销毁窗口的子孙窗口。
//参数: gkwin，目标窗口
//返回: 无
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

//----输出窗口redraw部分-------------------------------------------------------
//功能: 把所有窗口有修改的可视域输出至显示器，假设可视域已经准备好。
//参数: display，被扫描的显示器
//返回: 无
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

    frame_buf = display->frame_buffer;          //取帧缓冲窗口
    desktop_gkwin = display->desktop;
    topwin = display->z_topmost;
    if(frame_buf == NULL)   //无帧缓冲，无帧缓冲的显示器，必然没有窗口缓冲
    {
        //在没有缓冲区的情况下，绘制操作是不会生成redraw_clip的，只有窗口管理
        //操作才会要求重绘。
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
    }else                                           //有帧缓冲
    {
        gkwin = desktop_gkwin;
        while(1)
        {
            clip = gkwin->redraw_clip;
            if(clip != NULL)
            {
                if(gkwin->wm_bitmap == NULL)   //无窗口缓冲
                {
                    repaint.command = CN_GKUC_REPAINT;
                    repaint.gk_win = (void*)gkwin;
                    repaint.redraw_clip = clip;

                    __GK_UsercallChunnel((void*)&repaint,sizeof(struct GkucParaRepaint),
                                            1000*mS);

                    //等待用户完成重绘操作，超时(1秒)不候
                }else                                   //有窗口缓冲
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
        //以下把帧缓冲区输出到显示器
        clip = __GK_GetChangedClip(frame_buf);  //获取帧缓冲中被修改的可视域
        if(clip != NULL)
        {
            __GK_ShadingClear(frame_buf);
            frame_buf->redraw_clip = clip;
//          mirror = gkwin->disp->HostObj;
            do{
                if(display->framebuf_direct == false)
                {
                    //输出到screen
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

                //处理镜像显示
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

//----显示内容输出到显示器-----------------------------------------------------
//功能: 把所有显示器中需要输出的部分输出到显示器。
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void __gk_redraw_all(void)
{
    struct DisplayObj *Display;
    struct Object *current;

    current = OBJ_GetChild(s_ptDisplayDir);
    while(current != NULL)
    {
        Display = (struct DisplayObj*)OBJ_GetPrivate(current);
        __GK_GetRedrawClipAll(Display);//扫描取得新的需重绘的区域
        __GK_OutputRedraw(Display);    //重绘
        current = OBJ_ForeachChild(s_ptDisplayDir, current);
    }

}

//----重新刷新显示器----------------------------------------------------------
//功能: 本函数由GK_ApiRefreshDisplay调用引起。对于有framebuffer的显示器,本函数
//      的实现相当简单,就是把framebuffer重新copy到display和mirror而已.对无
//      framebuffer的显示器,就稍微复杂一些,需要把各窗口的可视域作为重绘域通过
//      GK_UsercallChunnel函数发到应用层, 请求应用层重绘。
//      无论哪种情况,本函数均不改变所有窗口的可视域与重绘域.
//参数: Display,须刷新的显示器
//返回: 无
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

    frame_buf = Display->frame_buffer;          //取帧缓冲窗口
    desktop_gkwin = Display->desktop;
    topwin = Display->z_topmost;
    if(frame_buf == NULL)   //无帧缓冲，无帧缓冲的显示器，必然没有窗口缓冲
    {
        //在没有缓冲区的情况下，直接把visible_clip作为重绘域上传。
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
                //等待用户完成重绘操作，超时(1秒)不候
//              Lock_SempPend(g_ptUsercallSemp,0);
            }
            if(gkwin != topwin)
                gkwin = gkwin->z_top;
            else
                break;
        }
    }else                                           //有帧缓冲
    {
        rect.left = 0;
        rect.top = 0;
        rect.right = Display->width - 1;
        rect.bottom = Display->height - 1;
        //以下把帧缓冲区输出到显示器
        if(Display->framebuf_direct == false)
        {
            //输出到screen
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
        //处理镜像显示
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

//----系统调用管道-------------------------------------------------------------
//功能: gui api与gui kernel的接口函数，把api所请求的绘制要求传递到gui kernel，在
//      mp版本中，可以考虑使用软中断实现。
//参数: param，参数地址
//      size，参数长度
//返回: 实际写入管道的数据量，0或者size。
//-----------------------------------------------------------------------------
extern bool_t WaitGkDraw(u32 timeout);
u16 __GK_SyscallChunnel(u16 command,u32 sync_time,void *param1,u16 size1,
                                                void *param2,u16 size2)
{
    u16 completed = 0;
    u8 buf[4];
    u32 base_time,rel_timeout = sync_time;
    base_time = (u32)DJY_GetSysTime();
    //管道访问互斥，用于多个上层应用并发调用之间的互斥
    if(Lock_MutexPend(g_tGkChunnel.syscall_mutex,rel_timeout))
    {
        while(1)
        {
            if((Ring_Capacity(&g_tGkChunnel.ring_syscall)
                        - Ring_Check(&g_tGkChunnel.ring_syscall)) <(u32)(size1+size2+4))
            {
                //管道容量不足，先执行管道内的命令：提升gkserver的优先级，令其立即执行，因
                //本线程也处于就绪态，gkserver在执行一个时间片后将返回，此时不管是否执行完毕，
                //都把gkserver线程恢复低优先级。

                //上层应用多次下发命令后，gk_server将一次处理，条件判一下，避免gk_server
                //空转
                if(Lock_SempQueryFree(g_ptSyscallSemp) == 0)
                    Lock_SempPost(g_ptSyscallSemp);
                WaitGkDraw(1);

                continue;       //再次检查管道容量
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
            //上层应用多次下发命令后，gk_server将一次处理，条件判一下，避免多次post导致
            //gk_server被反复激活
            if(Lock_SempQueryFree(g_ptSyscallSemp) == 0)
                Lock_SempPost(g_ptSyscallSemp);
            if(0 != sync_time)      //设定了等待时间
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
        Lock_MutexPost(g_tGkChunnel.syscall_mutex);    //管道访问互斥解除
    }
    else
        completed = 0;
    return completed;
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----Usercall消息发送----------------------------------------------------------
//功能: 本函数是gk_syscall_chunnle函数的逆操作函数，当gui kernel需要上层应用程序
//      完成某些操作时，调用本函数，把操作要求通过管道传送上去。
//参数: usercall_id: 消息ID
//      pdata: 消息参数
//      size: 消息参数内容字节数(不包括消息ID所占字节)
//返回: ture:成功; false:失败
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

//返回: 命令长度(字节数)
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
    if(g_ptGuiAppWaiting != NULL)     //gui app 等待中
    {
        if(g_ptGuiAppWaiting->event_status & CN_STS_SYNC_TIMEOUT)
            __DJY_ResumeDelay(g_ptGuiAppWaiting);    //如果事件在超时等待队列中，取出
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

    //下面看看是否要做优先级继承
    __DJY_FollowUpPrio(g_ptServerEvent);
    Int_RestoreAsynSignal();  //恢复中断，将触发上下文切换
    //检查从哪里返回，是超时还是同步事件完成。
    if(g_ptEventRunning->wakeup_from & CN_STS_SYNC_TIMEOUT)
    {//说明同步条件未到，从超时返回
        __DJY_RestorePrio( g_ptServerEvent );
        return false;
    }else
    {//说明是得到互斥量返回
        return true;
    }
}


//----gui kernel服务器---------------------------------------------------------
//功能: 从缓冲区取出显示请求，并处理之。
//参数: 无
//返回: 无
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
        //一次读取全部命令，因为发送时有互斥量保护，所以管道中的数据肯定是完整的
        //命令，不存在半条命令的可能。
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
//          Lock_SempPost(g_tGkChunnel.syscall_semp);    //释放管道
        offset = 0;
        while(num > offset)
        {
            //由于管道中的数据可能不对齐，故必须把数据copy出来，不能直接用指针
            //指向或强制类型转换
            command = draw_chunnel_buf[offset] + ((u16)draw_chunnel_buf[offset+1]<<8);
            len =   draw_chunnel_buf[offset+2] + ((u16)draw_chunnel_buf[offset+3]<<8);
            offset += 4;
//            sync_draw = draw_chunnel_buf[offset];
//            offset += 1;
            size= __ExecOneCommand(command,(u8 *)draw_chunnel_buf + offset);
            if(len != size)
                debug_printf("gui kernel","命令长度出错\n\r");
            offset +=len;
        }   //for while(num != offset)
    }   //for while(1)

}

