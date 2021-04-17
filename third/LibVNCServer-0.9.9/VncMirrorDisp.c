
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"
#include "endian.h"
#include "stdio.h"

#include <rfb/rfb.h>
#include "gkernel.h"
#include <gui/gk_display.h>
#include "VncOsPort.h"
#include "VncNetPort.h"

#define vncmin(a,b)      (a>b?b:a)
#define vncmax(a,b)      (a>b?a:b)


static struct DisplayObj *ptVncDisplay = NULL;
static rfbScreenInfo* pgRfbScreen = NULL;

// =============================================================================
// ��������:���껺���еĸ���clip���¸�cl�ͻ���
// �������:�ͻ���cl
// ���������
// ����ֵ  :true,���³ɹ���false����ʧ��
// ˵��    :�ڲ�ʹ�ú���������������֮��İ�ȫ���
// =============================================================================
//ע��BITMAP�����Ƿ��Ͳ��֣�������������
static bool_t  __SendBm2Client(rfbClientRec *cl, struct Rectangle *dst_rect,\
                               struct RectBitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    bool_t ret=true;
    int  x,y,x2,y2,w,h;
    int  bufoff;
    int  linepaddedbytes;
    int  pixelpaddedbytes;

    int ax1,ay1,ax2,ay2;
    int bx1,by1,bx2,by2;
    rfbFramebufferUpdateMsg *fu = (rfbFramebufferUpdateMsg *)cl->updateBuf;

    //��������Ȥ�Ĳ��֣����ߵĹ�������
    ax1 = dst_rect->left;
    ay1= dst_rect->top;
    ax2 =dst_rect->right;
    ay2 = dst_rect->bottom;

    bx1 = cl->reqregion.x;
    by1 = cl->reqregion.y;
    bx2 = bx1+ cl->reqregion.w;
    by2 = by1 + cl->reqregion.h;

    if((ax1>bx2)||(ax2<bx1)||(ay1> by2)||(ay2 < by1))
    {
        ret = false;
    }
    else if((ax1>=bx1)&&(ax2<=bx2)&&(ay1>=by1)&&(ay2<=by2)) //B����A
    {
        x = ax1;
        x2 = ax2;
        y = ay1;
        y2 = ay2;
    }
    else if((ax1<=bx1)&&(ax2>=bx2)&&(ay1<=by1)&&(ay2>=by2)) //A����B
    {
        x = bx1;
        x2 = bx2;
        y = by1;
        y2 = by2;
    }
    else                                                   //A B�ཻ
    {
        x = vncmax(ax1,bx1);
        x2 = vncmin(ax2,bx2);
        y = vncmax(ay1,by1);
        y2 = vncmin(ay2,by2);
    }

    if(ret)
    {
        w = (int)(x2-x);
        h = (int)(y2-y);

        pixelpaddedbytes = cl->screen->serverFormat.bitsPerPixel/8;
        linepaddedbytes = src_bitmap->linebytes;
        bufoff = (y-ay1+ysrc)*linepaddedbytes + (x-ax1+xsrc)*pixelpaddedbytes;
        cl->screen->frameBuffer=src_bitmap->bm_bits+bufoff;
        cl->screen->paddedWidthInBytes = linepaddedbytes;

        fu->type = rfbFramebufferUpdate;
        fu->nRects = htons(1);
        cl->ublen = sz_rfbFramebufferUpdateMsg;
        //for the test
//      cl->preferredEncoding = rfbEncodingRaw;
        switch (cl->preferredEncoding)
        {
            case rfbEncodingRaw:
                if (!rfbSendRectEncodingRaw(cl, x, y, w, h))
                    printk("%s:Client Send Failed\n\r",__FUNCTION__);
                break;
            case rfbEncodingRRE:
                if (!rfbSendRectEncodingRRE(cl, x, y, w, h))
                    printk("%s:Client Send Failed\n\r",__FUNCTION__);
                break;
            case rfbEncodingCoRRE:
                if (!rfbSendRectEncodingCoRRE(cl, x, y, w, h))
                    printk("%s:Client Send Failed\n\r",__FUNCTION__);
                break;
            case rfbEncodingHextile:
                if (!rfbSendRectEncodingHextile(cl, x, y, w, h))
                    printk("%s:Client Send Failed\n\r",__FUNCTION__);
                break;
            case rfbEncodingUltra:
                if (!rfbSendRectEncodingUltra(cl, x, y, w, h))
                    printk("%s:Client Send Failed\n\r",__FUNCTION__);
                break;
            case rfbEncodingZlib:
                if (!rfbSendRectEncodingZlib(cl, x, y, w, h))
                    printk("%s:Client Send Failed\n\r",__FUNCTION__);
                break;
           case rfbEncodingZRLE:
           case rfbEncodingZYWRLE:
               if (!rfbSendRectEncodingZRLE(cl, x, y, w, h))
                    printk("%s:Client Send Failed\n\r",__FUNCTION__);
               break;
           default:
                if (!rfbSendRectEncodingRaw(cl, x, y, w, h))
                    printk("%s:Client Send Failed\n\r",__FUNCTION__);
                break;
        }
        if (!rfbSendUpdateBuf(cl))
        {
            printk("%s:Client Send Failed\n\r",__FUNCTION__);
        }
    }
    return ret ;
}


bool_t __Bm2Screen(struct Rectangle *dst_rect,\
                  struct RectBitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    rfbClientRec *cl = NULL, *cl_nxt = NULL;
    if(NULL == pgRfbScreen)
    {
        return true;
    }
    if(src_bitmap->PixelFormat != ptVncDisplay->pixel_format)
    {
        //we just deal the same color type
        return true;
    }
    LOCK(pgRfbScreen->clientsync);
    cl=pgRfbScreen->clientHead;
    while(cl)
    {
        //chk every client, and send the bitmap to it
        cl_nxt = cl->next;
        if(cl->initdone)
        {
            //if the client is ready , then update it
            __SendBm2Client(cl,dst_rect,src_bitmap,xsrc,ysrc);
            cl->initdone = false;   //������һ��
        }
        cl=cl_nxt;
    }
    UNLOCK(pgRfbScreen->clientsync);
    return true;
}
// =============================================================================
// ��������:��װ������ʾ��
// �������:screen��rfbserver����ʾ��
// ���������
// ����ֵ  :true:�ɹ�  false��ʧ��
// ˵��    :�ڰ�װ������ʾ����ʱ�����ǻ��ȡ����ʾ����һЩ��������˿��Գ�ʼ�����ǵ�
//          screen�Ĳ������ȷ�˵�ߴ��Լ����ظ�ʽ���Լ�screen��buf�����룬ͬʱ����Ҳ
//        Ҫ���Լ���clippool������͵ײ��clip��˵����Ĺ�ϵ
// =============================================================================
bool_t VncDispInstall(rfbScreenInfo* rfbserver,const char *lcdname)
{
    bool_t  result;
    struct DisplayObj *pBaseDisplay;
    struct GkWinObj   *desktop;

    result = false;
    desktop = GK_GetDesktop(lcdname);
    if(NULL==desktop)//�Ҳ������档
    {
        goto EXIT_PARAERR;
    }
    pBaseDisplay=desktop->disp;
    if(NULL==pBaseDisplay)//�Ҳ�������ʾ��
    {
        goto EXIT_PARAERR;
    }
    ptVncDisplay=M_Malloc(sizeof(struct DisplayObj),0);
    if(NULL==ptVncDisplay)
    {
        printk("%s :No Mem For VncDisplay\n\r",__FUNCTION__);
        goto EXIT_VNCDISPLAYMEM;
    }
    memset(ptVncDisplay,0,sizeof(struct DisplayObj));

    ptVncDisplay->pixel_format = pBaseDisplay->pixel_format;
    ptVncDisplay->width = pBaseDisplay->width;
    ptVncDisplay->height = pBaseDisplay->height;
    ptVncDisplay->draw.bm_to_screen=__Bm2Screen;

    //�����ɫ������
    rfbserver->width=ptVncDisplay->width;
    rfbserver->height=ptVncDisplay->height;

    switch(ptVncDisplay->pixel_format)//Ŀǰ���о��⼸�ְɣ����������в���
    {
        case CN_SYS_PF_RGB332://332��ʽ
            rfbserver->depth = 8;
            rfbserver->bitsPerPixel = 8;
            rfbserver->serverFormat.bitsPerPixel = 8;
            rfbserver->serverFormat.trueColour=true;
            rfbserver->serverFormat.redShift=5;
            rfbserver->serverFormat.greenShift=2;
            rfbserver->serverFormat.blueShift=0;
            rfbserver->serverFormat.redMax=((1<<3)-1);
            rfbserver->serverFormat.greenMax=((1<<3)-1);
            rfbserver->serverFormat.blueMax=((1<<2)-1);
            rfbserver->serverFormat.depth=8;
            break;
        case CN_SYS_PF_RGB565://565��ʽ
            rfbserver->depth = 16;
            rfbserver->bitsPerPixel = 16;
            rfbserver->serverFormat.bitsPerPixel = 16;
            rfbserver->serverFormat.trueColour=true;
            rfbserver->serverFormat.redShift=11;
            rfbserver->serverFormat.greenShift=5;
            rfbserver->serverFormat.blueShift=0;
            rfbserver->serverFormat.redMax=((1<<5)-1);
            rfbserver->serverFormat.greenMax=((1<<6)-1);
            rfbserver->serverFormat.blueMax=((1<<5)-1);
            rfbserver->serverFormat.depth=16;
            break;
        case CN_SYS_PF_ERGB8888://���ɫ888
            rfbserver->depth = 32;
            rfbserver->bitsPerPixel = 32;
            rfbserver->serverFormat.bitsPerPixel = 32;
            rfbserver->serverFormat.trueColour=true;
            rfbserver->serverFormat.redShift=16;
            rfbserver->serverFormat.greenShift=8;
            rfbserver->serverFormat.blueShift=0;
            rfbserver->serverFormat.redMax=((1<<8)-1);
            rfbserver->serverFormat.greenMax=((1<<8)-1);
            rfbserver->serverFormat.blueMax=((1<<8)-1);
            rfbserver->serverFormat.depth=24;
            break;
        default:
            rfbserver->depth = 8;
            rfbserver->bitsPerPixel = 8;
            rfbserver->serverFormat.bitsPerPixel = 8;
            rfbserver->serverFormat.trueColour=true;
            rfbserver->serverFormat.redShift=5;
            rfbserver->serverFormat.greenShift=2;
            rfbserver->serverFormat.blueShift=0;
            rfbserver->serverFormat.redMax=((1<<3)-1);
            rfbserver->serverFormat.greenMax=((1<<3)-1);
            rfbserver->serverFormat.blueMax=((1<<2)-1);
            rfbserver->serverFormat.depth=8;
            break;
    }
    rfbserver->serverFormat.bigEndian=CN_CFG_BYTE_ORDER;
    rfbserver->paddedWidthInBytes=(rfbserver->bitsPerPixel/8)*rfbserver->width;
    rfbserver->sizeInBytes = rfbserver->paddedWidthInBytes*rfbserver->height;

    result= GK_InstallDisplayMirror(pBaseDisplay,ptVncDisplay,"VncDisplay");
    if(false == result)//��װʧ��
    {
        printk("%s :Install VNC DISPLAY FAILED\n\r",__FUNCTION__);
        goto EXIT_DSIPLAYINSTALL;
    }
    printk("%s :VncDisplayInit SUCCESS\n\r",__FUNCTION__);
    pgRfbScreen = rfbserver;
    return result;

EXIT_DSIPLAYINSTALL:
    free(ptVncDisplay);
    ptVncDisplay = NULL;
EXIT_VNCDISPLAYMEM:
EXIT_PARAERR:
    result = false;
    printk("%s :VncDisplayInit FAILED\n\r",__FUNCTION__);
    return result;
}




