#include <stdint.h>
#include <stddef.h>
#include <gkernel.h>
#include <loc_string.h>
#include <gdd.h>
#include <gdd_widget.h>
#include <gui\gdd_cursor.h>

#include    "../gdd_private.h"

#include    <string.h>
#include    <_ansi.h>
/*============================================================================*/
#define ID_INSERT   0x1100
#define ID_COLOR    0x1102
#define CN_MAX_CHAR_ONE_LINE   128      //一行最多128个字符
#define CN_MAX_LINES           64       //最多64行
#define ID_ENTER    0x1000
typedef struct
{
   u8 cur_byte_idx;       //当前行字符数;
   bool_t x_scrollbar;    //是否开启水平方向滚动条
   bool_t y_scrollbar;    //是否开启垂直方向滚动条
   u16 height;            //高像素
   u16 width;             //宽像素
   u16 cur_line_index;    //当前的行数
   u16 len;               //RichTextBox中所有字符串字节数
   char *pcurchar;        //当前指向的字符
   char *text;            //RichTextBox中所有的字符组成的字符串
   u8 CursorLoc;          //光标所在id位置
   u16 idwidth;           //光标所在字符处，距左侧宽度
   u16 idheight;           //光标所在字符处，距顶部高度
}RichTextBox;


static  s8 cfg_idx=0;
static  s8 bBorder=FALSE;
static  s8 bBKGND=FALSE;
static  s8 bColor=FALSE;
//static  ptu32_t ButtonL_Up(struct WindowMsg *pMsg)
//

#include <stdio.h>

//这是文本框显示的原字符串
char *RichTextBox_text="蜀道之难，难于上青天！\n蚕丛及鱼凫，开国何茫然！尔来四万八千岁，不与秦塞通人烟。";
char *RichTextBox_add="插入插入\n插入插入。";      //这是按下按键后插入的字符串
static char RichTextBox_temp[300];                      //用来存放经过处理后的字符串
static char RichTextBox_temp_str[300];

// =============================================================================
// 函数功能: 获取字符串长度字节数,RichTextBox能够显示多行信息。
// 输入参数: char *str:字符串
// 输出参数: 无。
// 返回值  :获取到有效字符字节数。
// =============================================================================
static s16 __Widget_GetValidStrLen(char *str)
{
    s16 str_len=0;
    if(str==NULL)
        return -1;
    str_len=strlen(str);
    return str_len;
}


// =============================================================================
// 函数功能: 获取字符串中字符数及总的字符所占像素数。
// 输入参数: str:字符串指针
//          pChunm:字符数指针,输入参数为NULL时则不计算字符数。
//          pChWidthSum:字符所占总像素数指针，输入参数为NULL时则不计算字符所占总像素宽度.
// 输出参数: 无。
// 返回值  :成功则返回true，失败则返回false.
// =============================================================================
static bool_t __Widget_GetValidStrInfo(char *str,u16 *pChunm,u16 *pChWidthSum)
{
    u16 cnt=0,chwidthsum=0;
    s16 str_len=0;
    s32 len,chwidth=0;
    struct FontObj* cur_font;
    struct Charset* cur_enc;
    u32 wc;
    if(str==NULL)
        return false;
    str_len=__Widget_GetValidStrLen(str);
    if(str_len==-1)
        return false;
     //计算字符串中字符数
     cur_font = Font_GetCurFont();
     cur_enc = Charset_NlsGetCurCharset();
     for(; str_len > 0;)
     {
        len= cur_enc->MbToUcs4(&wc, str, -1);
        if(len == -1)
        {   // 无效字符
            str_len--;
            str++;
        }
        else if(len == 0)
        {
             break;
        }
        else
        {          // 有效字符
            str += len;
            str_len -= len;
            if(pChunm!=NULL)  //统计有几个字符（可以识别多字节字符）
            {
              cnt++;
            }
            if(pChWidthSum!=NULL)
            {
              chwidth=cur_font->GetCharWidth(wc);
              chwidthsum+=chwidth;
            }
            continue;
        }
     }

     *pChWidthSum=chwidthsum;
     *pChunm=cnt;
     return true;
}


// =============================================================================
// 函数功能: 获取字符串中从开始到指定编号的字符总的字节数.
// 输入参数: str:字符串指针
//           idx:字符串中指定字符序号，从1开始，依次往后增加.
// 输出参数: 无
// 返回值  :指定字符地址.
// =============================================================================
static s16 __Widget_CharToBytes(char *str,u8 num)
{
     u16 bytes=0;
     s16 str_len=0;
     u8 cnt=0;
     u32 wc;
     s32 len;
     struct Charset* cur_enc;
     if(str==NULL)
          return -1;
     str_len=__Widget_GetValidStrLen(str);
     if(str_len==-1)
        return -1;
     if(num==0)
         return -1;
     cur_enc = Charset_NlsGetCurCharset();
     for(; str_len > 0;)
     {
        len= cur_enc->MbToUcs4(&wc, str, -1);//计算每一个字符有多长 ，累加起来
        if(len == -1)
        {     // 无效字符
            str_len--;
            str++;
            bytes++;
        }
        else if(len == 0)
        {
            break;
        }
        else
        {          // 有效字符
             str += len;
             str_len -= len;
             bytes+=len;
             cnt++;
         if(cnt==num)
         {
             return bytes;
         }
              continue;
         }
     }
     return bytes;
}

// =============================================================================
// 功能:在RichTextBox指定光标位置处开始插入字符串
// 参数:text，原字符串
//      add，待插入的字符串
//      hwnd,文本框窗体句柄；
// 输出参数: 无。
// 返回值  :成功则返回true，失败则返回false.
// =============================================================================
static char * __Widget_RichTextBoxInsertChar(char *text,char *add,HWND hwnd)
{
    u8 i,j;
    RichTextBox *pRTB;
    u8 num=0;
    s16 str_len=0;
    s16 len=0;
    pRTB = (RichTextBox *)hwnd->PrivateData;
    str_len=__Widget_GetValidStrLen(text);
    len=__Widget_CharToBytes(text,pRTB->CursorLoc);//此处修改字符串插入位置
    if(len==-1)
        len=0;

    for(i=0,j=0;j<str_len;i++,j++)
    {
        if(i==len)
        {
           while(1)
           {
               RichTextBox_temp[i]=*(add+num);
               i++;
               num++;
               if(*(add+num)=='\0')
                   break;
           }
        }
        RichTextBox_temp[i]=*(text+j);
    }
    RichTextBox_temp[i]='\0';
    return RichTextBox_temp;

}


// =============================================================================
// 函数功能:在RichTextBox中，按照要求插入换行符使字符串能够多行显示
// 输入参数:  hwnd,文本框窗体句柄；
//           text,字符串指针.
// 输出参数: 无。
// 返回值  :返回经过处理，插入了换行符的字符串指针
// =============================================================================
static char *__Widget_RichTextBoxLineFeed(char *text,HWND hwnd)
{
         u8 i,j;
         s8 num=0;
         s16 str_len=0;
         RichTextBox *pRTB;

         pRTB = (RichTextBox *)hwnd->PrivateData;
         str_len = __Widget_GetValidStrLen(text);

         for(i=0,j=0;j<str_len;i++,j++,num++)
         {
             if(*(text+j)=='\n')
             {
                 num=-1;
             }
             if(num==pRTB->cur_byte_idx)
             {
                 RichTextBox_temp_str[i]='\n';
                 i++;
                 num=0;
             }
             RichTextBox_temp_str[i]=*(text+j);
         }
         RichTextBox_temp_str[i]='\0';
         return RichTextBox_temp_str;


}

// =============================================================================
// 函数功能: 获取字符串到指定编号字符所占的像素宽度.
// 输入参数: str:字符串指针
//          idx:字符编号.
// 输出参数: 无。
// 返回值  :字符像素总宽度.
// =============================================================================
static s16 __Widget_GetStrWidth(char *str,u16 idx)
{
    struct FontObj* cur_font;
    struct Charset* cur_enc;
    u32 wc;
    s32 len,chwidth=0;
    u16 cnt=0,chwidthsum=0;
    s16 str_len=0;
    if(str==NULL)
         return -1;
    str_len=__Widget_GetValidStrLen(str);
    if(str_len==-1)
         return -1;
    if(idx==0)
        return 0;
    //计算字符串中字符数
    cur_font = Font_GetCurFont();
    cur_enc = Charset_NlsGetCurCharset();
    for(; str_len > 0;)
    {
        len= cur_enc->MbToUcs4(&wc, str, -1);
        if(len == -1)
        {   // 无效字符
            str_len--;
            str++;
        }
        else if(len == 0)
             break;
        else
        {          // 有效字符
            str += len;
            str_len -= len;
            cnt++;
            chwidth=cur_font->GetCharWidth(wc);
            chwidthsum+=chwidth;
            if(cnt==idx)
                break;
            else
               continue;
        }
    }

     return chwidthsum;
}



// =============================================================================
// 函数功能: 移动光标.
// 输入参数: hwnd:文本框窗口句柄;
//          idx:文本框字符编号.
// 输出参数: 无。
// 返回值  :成功则返回true，失败则返回false.
// =============================================================================
static bool_t __Widget_MoveCursor(HWND hwnd,u8 idx)
{
    //内部调用，无需进行参数检查
    RichTextBox *pRTB;
    char *str;
    RECT rc;
    u16 width;
    s32 x,y;
    pRTB=(RichTextBox *)GDD_GetWindowPrivateData(hwnd);
    if(pRTB==NULL)
        return false;
    GDD_GetClientRectToScreen(hwnd,&rc);
    str=RichTextBox_text;
    if(str==NULL)
        return false;
    if(idx==0)
    {
        x=rc.left+1;
    }
    else
    {
        width=pRTB->idwidth;
        x=rc.left+width+1;
    }
    y=pRTB->idheight;
    GDD_CursorMove(x,y);

//    GDD_MoveWindow(g_CursorHwnd,x,y);
    GDD_UpdateDisplay(hwnd);
//  GDD_PostMessage(hwnd, MSG_SYNC_DISPLAY, 0, 0);
    return true;
}
// =============================================================================
// 函数功能: 根据触摸点坐标获取文本框中字符编号
// 输入参数: hwnd:文本框窗口句柄
//           x:触摸点x方向坐标
//           y:触摸点y方向坐标
// 输出参数: 无。
// 返回值  :文本框中字符编号.
// =============================================================================
static s16 __Widget_FindIdx(HWND hwnd,u16 x,u16 y)
{
    char *str;
    RichTextBox *pRTB;


    //计算行数所需定义
    struct FontObj* cur_font;
    s32 count,id=0;
    s32 linenum=0;
    s32 numinline[10];
    const char *line=RichTextBox_text;
    char *linenext;


    RECT rc;
    s16 tmp=0,val1=0,val2=0;
    u16 chnum,idx,width=0;
    s32 line_count,line_h,y0;


    str=RichTextBox_text;
    if(str==NULL)
        return -1;
    pRTB=(RichTextBox *)GDD_GetWindowPrivateData(hwnd);

//计算每行高度
    cur_font = Font_GetCurFont();
    line_h = Font_GetFontLineHeight(cur_font);

//计算行数部分
    count=0;
        while(1)
        {
            linenext = mbstrchr(line, "\n", &count);
            if(linenext != NULL)
            {
                numinline[linenum]=count+1;
                linenum++;
                line = linenext+1;
            }
            else
            {
                if(count != 0)
                {
                    numinline[linenum]=count+1;
                    linenum++;
                }
                break;
            }
        }



//计算坐标在第几行       (默认左对齐，水平居中)
         GDD_GetClientRectToScreen(hwnd,&rc);
         y0 =rc.top;
         y0 += ((GDD_RectH(&rc)-(linenum*line_h)-((linenum-1)*line_h))/2)-(0.5*line_h);//初始高度定在第一行上移半行的地方
         for(line_count=1;line_count<=linenum;line_count++)
         {
             tmp=y0+(line_count*2)*line_h;//算上行间距，每行高度有两倍的字符高度 ，字一行，上下间距各半行
             if(tmp>y)
             {
                  break;
             }
             id+=numinline[line_count-1];
         }
         pRTB->idheight=y0+(0.5*line_h)+(line_count-1)*2*line_h;



 //通过像素宽度计算坐标在那一行的第几个数
    if(pRTB==NULL)
        return -1;
    __Widget_GetValidStrInfo(str,&chnum,NULL);

    for(idx=id;idx<(id+numinline[line_count-1]);idx++)
    {
        width=__Widget_GetStrWidth(str, idx)-__Widget_GetStrWidth(str, id);
        tmp=rc.left+width;
        if(tmp>x)
        {
           break;
        }
    }
    val1=tmp-x;
    width=__Widget_GetStrWidth(str, idx-1)-__Widget_GetStrWidth(str, id);
    tmp=rc.left+width;
    val2=x-tmp;
    if(val1>val2)
    {
        pRTB->idwidth=__Widget_GetStrWidth(str, idx-1)-__Widget_GetStrWidth(str, id);
        return idx-1;
    }
    else
    {
        pRTB->idwidth=__Widget_GetStrWidth(str, idx)-__Widget_GetStrWidth(str, id);
        return idx;
    }
}


// =============================================================================
// 函数功能: RichTextBox控件触摸屏按下响应函数。响应MSG_TOUCH_DOWN消息
// 输入参数: pMsg,窗体消息结构体指针
// 输出参数: 无。
// 返回值  :成功返回true,失败则返回false。
// =============================================================================
static bool_t __Widget_RichTextBoxTouchDown(struct WindowMsg *pMsg)
{
    HWND hwnd;
    u32 loc;
    u16 chnum,idx,CharWidth,x,y;
    RichTextBox *pRTB;
    char *str;
    s32 tmp;
    RECT rc;

    str=RichTextBox_text;
    hwnd =pMsg->hwnd;

    pRTB=(RichTextBox *)GDD_GetWindowPrivateData(hwnd);
    if(pRTB==NULL)
        return false;
    //不可编辑的文本框(Lable不响应该事件)

    __Widget_GetValidStrInfo(str,&chnum,&CharWidth);
    //在文本框上
    loc=pMsg->Param2;   //获取触摸点信息
    x=loc;
    y=loc>>16;
    if(str==NULL)
    {
         pRTB->CursorLoc=0;
        __Widget_MoveCursor(hwnd,0);
    }
    else
    {
        __Widget_GetValidStrInfo(str,NULL,&CharWidth);
        GDD_GetWindowRect(hwnd,&rc);
        tmp=rc.left+CharWidth;
        if(x>tmp)
        {
            pRTB->CursorLoc=chnum;
            __Widget_MoveCursor(hwnd,chnum);
        }
        else
        {
           idx=__Widget_FindIdx(hwnd,x,y);
           pRTB->CursorLoc=idx;
           __Widget_MoveCursor(hwnd,idx);
        }
    }
    GDD_SetWindowPrivateData(hwnd,(ptu32_t)pRTB);

    GDD_CursorSetHost(hwnd);

    GDD_SetFocusWindow(hwnd);

    return true;
}

// =============================================================================
// 函数功能: RichTextBox控件子控件发送的通知。响应MSG_NOTIFY消息
// 输入参数: pMsg,窗体消息结构体指针
// 输出参数: 无。
// 返回值  :成功返回true,失败则返回false。
// =============================================================================
static ptu32_t __Widget_RichTextBoxNotify(struct WindowMsg *pMsg)
{
        HWND hwnd;
        u16 event,id;

        hwnd =pMsg->hwnd;

        event =HI16(pMsg->Param1);
        id =LO16(pMsg->Param1);

        switch (id)
        {
        case ID_INSERT://换行
             if(event==CBN_SELECTED)
             {
            __Widget_RichTextBoxInsertChar(RichTextBox_text,RichTextBox_add,hwnd);
            GDD_InvalidateWindow(hwnd,FALSE);
             }
            break;

        case ID_COLOR://改变颜色
            if(event==CBN_SELECTED)
                {
                    bColor =TRUE;
                }
            if(event==CBN_UNSELECTED)
                {
                    bColor =FALSE;
                }
            GDD_InvalidateWindow(hwnd,true);
            break;
            default:break;
        }
    return true;
}

//创建主窗口
// =============================================================================
// 函数功能: RichTextBox控件创建函数。响应MSG_CREATE消息
// 输入参数: pMsg,窗体消息结构体指针
// 输出参数: 无。
// 返回值  :成功返回true,失败则返回false。
// =============================================================================
static ptu32_t __Widget_RichTextBoxCreate(struct WindowMsg *pMsg)
{
    HWND hwnd;
    RECT rc0;
    RichTextBox *pRTB;
    hwnd =pMsg->hwnd;
    cfg_idx =0;

    if(pMsg->Param1==0)
        {
            pRTB=(RichTextBox *)malloc(sizeof(RichTextBox));
            if(pRTB==NULL)
                return false;
            memset(pRTB, 0, sizeof(RichTextBox));
            pRTB->cur_byte_idx=16;//此处修改每一排的字符数
            GDD_SetWindowPrivateData(hwnd,(ptu32_t)pRTB);
        }
    GDD_GetClientRect(hwnd,&rc0);
//    GDD_CursorInit();
    Widget_CreateButton("插入换行",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE,GDD_RectW(&rc0)-64,GDD_RectH(&rc0)-60,60,24,hwnd,ID_INSERT,0,NULL);
    Widget_CreateButton("改变颜色",WS_CHILD|BS_HOLD|WS_BORDER|WS_VISIBLE,GDD_RectW(&rc0)-64,GDD_RectH(&rc0)-30,60,24,hwnd,ID_COLOR,0,NULL);
    return true;
}

//窗口背景擦除
static ptu32_t __Widget_RichTextBoxErasebkgnd(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC  hdc;
    RECT rc0;

    hwnd =pMsg->hwnd;
    hdc =(HDC)pMsg->Param1;
    GDD_GetClientRect(hwnd,&rc0);
    GDD_SetFillColor(hdc,RGB(200,200,200));
    GDD_FillRect(hdc,&rc0);

    return true;
}

//绘图消息
// =============================================================================
// 函数功能: RichTextBox控件绘制函数。响应MMSG_PAINT消息
// 输入参数: pMsg,窗体消息指针
// 输出参数: 无。
// 返回值  :成功则返回true，失败则返回false.
// =============================================================================
static ptu32_t __Widget_RichTextBoxPaint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC  hdc;
    RECT rc0,rc;
    u32 i;
    hwnd =pMsg->hwnd;

    hdc = GDD_BeginPaint(hwnd);
    if(hdc == NULL)
        return false;
    GDD_GetClientRect(hwnd,&rc0);

    GDD_SetRect(&rc,2,2,GDD_RectW(&rc0)-2*2,GDD_RectH(&rc0)-10);

    if(bColor==true)
       {
        GDD_SetFillColor(hdc,RGB(200,200,0));
       }
    else
        GDD_SetFillColor(hdc,RGB(0,200,0));

    GDD_FillRect(hdc,&rc);

    GDD_SetTextColor(hdc,RGB(1,0,0));
    GDD_SetDrawColor(hdc,RGB(255,255,255));
//    GDD_SetFillColor(hdc,RGB(200,0,240));

    i=DT_LEFT|DT_VCENTER;
    if(bBorder)
    {
        i |= DT_BORDER;
    }
    if(bBKGND)
    {
        i |= DT_BKGND;
    }
//    text=__Widget_RichTextBoxInsertChar(text,add);
    __Widget_RichTextBoxLineFeed(RichTextBox_temp,hwnd);
    GDD_DrawText(hdc,RichTextBox_temp_str,-1,&rc,i);
    GDD_EndPaint(hwnd,hdc);

    return true;
}

//窗口背景擦除


//消息处理函数表
//消息处理函数表
static struct MsgProcTable s_gDrawTextMsgTable[] =
{
    {MSG_CREATE,__Widget_RichTextBoxCreate},         //主窗口创建消息
//    {MSG_TIMER,HmiTimer},           //定时器消息
    {MSG_NOTIFY,__Widget_RichTextBoxNotify},         //子控件发来的通知消息
    {MSG_TOUCH_DOWN,__Widget_RichTextBoxTouchDown},
    {MSG_PAINT,__Widget_RichTextBoxPaint},           //绘制消息
//    {MSG_ERASEBKGND,HmiErasebkgnd}, ///窗口背景擦除; Param1:绘图上下文;
};
static struct MsgTableLink  s_gDrawTextDemoMsgLink;


void    Widget_CreateRichTextBox(void)
{
    s_gDrawTextDemoMsgLink.MsgNum = sizeof(s_gDrawTextMsgTable) / sizeof(struct MsgProcTable);
    s_gDrawTextDemoMsgLink.myTable = (struct MsgProcTable *)&s_gDrawTextMsgTable;
    GDD_CreateGuiApp("DrawText", &s_gDrawTextDemoMsgLink, 0x1000, CN_WINBUF_PARENT, 0);
    GDD_WaitGuiAppExit("DrawText");
}
