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
//所属模块: GDD
//作者:  zhb.
//版本：V1.0.0
//文件描述: 按钮控件实现
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2016-6-14
//   作者:  zhb.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//---------------------------------

#include  "gdd.h"
#include  "../gdd_private.h"
#include  <font/font.h>
#include  <gdd_widget.h>


#define CN_CHAR_NUM_MAX                 255
#define CN_CANCLE_KEY                   0xA3

// =============================================================================
// 函数功能: 获取字符串长度字节数,TextBox只能显示单行信息，首先检查字符串,如果遇到
//           \n(换行符)则结束，截取换行符之前的字符作为有效的字符串.
// 输入参数: char *str:字符串
// 输出参数: 无。
// 返回值  :获取到有效字符字节数。
// =============================================================================
static s16 __Widget_GetValidStrLen(struct Charset * myCharset, char *str)
{
    u8 cnt=0;
    char ch;
    s16 str_len=0;
    if(str==NULL)
        return -1;
    cnt=GDD_GetStrLineCount(myCharset,str);
    if(cnt>1)
    {
        while(1)
        {
            ch=*str;
            if(ch!='\n')
            {
                str_len++;
                str++;
            }
            else
            {
                *(str+1)='\0';  //todo: 如果是const串怎么办？
                break;
            }
        }
    }
    else
    {
        str_len=strlen(str);
    }
    return str_len;
}

// =============================================================================
// 函数功能: 获取字符串中指定编号的字符，编号从1,2,3......开始.
// 输入参数: str:字符串指针
//           idx:字符编号
// 输出参数:
// 返回值  :ucs4字符
// =============================================================================
static u32 __Widget_GetCharByIndex(char *str,u8 idx)
{
    s16 str_len=0,len=0;
//    struct FontObj* cur_font;
    struct Charset* cur_enc;
    u32 wc=0;
    u8 cnt=0;
    if(str==NULL)
        return false;
    cur_enc = Charset_NlsGetCurCharset();
    str_len=__Widget_GetValidStrLen(cur_enc, str);
    if(str_len==-1)
        return false;
     //计算字符串中字符数
//    cur_font = Font_GetCurFont();
    for(; str_len > 0;)
    {
       len= cur_enc->MbToUcs4(&wc, str, -1);
       if(len == -1)
       {  // 无效字符
           str_len--;
           str++;
       }
       else if(len == 0)
       {
            break;
       }
       else
       {    // 有效字符
           str += len;
           str_len -= len;
           cnt++;
           if(cnt==idx)
           {
               return wc;
           }
        }
    }
    return wc;

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
    cur_enc = Charset_NlsGetCurCharset();//获取当前字符集
    str_len=__Widget_GetValidStrLen(cur_enc, str);//获取字符串长度字节数
    if(str_len==-1)
        return false;
     //计算字符串中字符数
     cur_font = Font_GetCurFont();//获取字体
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
            if(pChunm!=NULL)
            {
              cnt++;
            }
            if(pChWidthSum!=NULL)
            {
              chwidth=cur_font->GetCharWidth(wc);//该字体中此字所占的像素有多少

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
    cur_enc = Charset_NlsGetCurCharset();
    str_len=__Widget_GetValidStrLen(cur_enc, str);//计算字符串中字符数
    if(str_len==-1)
         return -1;
    if(idx==0)
        return 0;
    //计算字符串中字符数
    cur_font = Font_GetCurFont();//字体
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
    TextBox *pTB;
    HDC hdc;
    char *str;
    RECT rc,rc0;
    u16 width;
    s32 x,y;
    pTB=(TextBox *)GDD_GetWindowPrivateData(hwnd);//获得私有数据
    if(pTB==NULL)
        return false;
    GDD_GetClientRectToScreen(hwnd,&rc);//转换坐标
    str=hwnd->Text;
    if(str==NULL)
        return false;
    if(idx==0)
    {
        x=rc.left+1;
    }
    else
    {
        width=__Widget_GetStrWidth(str,idx);
        x=rc.left+width+1;
    }
    GDD_GetClientRect(hwnd,&rc0);
    GDD_InflateRect(&rc0,-1,-1);
    hdc =GDD_GetClientDC(hwnd);
    GDD_AdjustTextRect(hdc,str,pTB->ChNum,&rc0,DT_VCENTER|DT_LEFT);
    GDD_DeleteDC(hdc);
    y=rc.top+rc0.top+1;
    GDD_CursorMove(x,y);

//    GDD_MoveWindow(g_CursorHwnd,x,y);
    GDD_SyncShow(hwnd);
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
    TextBox *pTB;
    RECT rc;
    s16 tmp=0,val1=0,val2=0;
    u16 chnum,idx,width=0;
    str=hwnd->Text;
    if(str==NULL)
        return -1;
    pTB=(TextBox *)GDD_GetWindowPrivateData(hwnd);
    if(pTB==NULL)
        return -1;
    chnum=pTB->ChNum;
    GDD_GetClientRectToScreen(hwnd,&rc);
    for(idx=1;idx<chnum;idx++)
    {
        width=__Widget_GetStrWidth(str, idx);
        tmp=rc.left+width;
        if(tmp>x)
        {
           break;
        }
    }
    val1=tmp-x;
    width=__Widget_GetStrWidth(str, idx-1);
    tmp=rc.left+width;
    val2=x-tmp;
    if(val1>val2)
    {
        return idx-1;
    }
    else
    {
        return idx;
    }
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
//     struct FontObj* cur_font;
     struct Charset* cur_enc;
     if(str==NULL)
          return -1;
     cur_enc = Charset_NlsGetCurCharset();
     str_len=__Widget_GetValidStrLen(cur_enc, str);
     if(str_len==-1)
        return -1;
     if(num==0)
         return -1;
//     cur_font = Font_GetCurFont();
     for(; str_len > 0;)
     {
        len= cur_enc->MbToUcs4(&wc, str, -1);
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
// 函数功能:在TextBox中添加字符串
// 输入参数: hwnd:文本框窗体句柄
//          str:字符串指针
// 输出参数: 无。
// 返回值  :成功则返回true，失败则返回false.
// =============================================================================
static bool_t __Widget_TextBoxAddChar(HWND hwnd,char *str )
{
     char *text;
     s16 len=0,str_len=0;
     u16 cnt,num=0;
     s16 bytes;
     u8 i;
     TextBox *pTB;
     bool_t ret;
     if(hwnd==NULL)
        return false;
     if(str==NULL)
        return false;
     pTB=(TextBox *)GDD_GetWindowPrivateData(hwnd);
     if(pTB==NULL)
        return false;
    //检查文本框编辑属性
     if(pTB->EditProperty==WS_TEXTBOX_R_O)
        return false;
     text=hwnd->Text;
     len=__Widget_GetValidStrLen(Charset_NlsGetCurCharset(), text);
     if(len==-1)
          return false;
     //检查一下str的合法性
      str_len=__Widget_GetValidStrLen(Charset_NlsGetCurCharset(), str);
      if(str_len==-1)
          return false;
      ret=__Widget_GetValidStrInfo(str,&num,NULL);
      if(!ret)
          return false;
      //字符串原有的字符数
      cnt=pTB->ChNum;
      if(cnt+num>CN_CHAR_NUM_MAX)
      {
           num=CN_CHAR_NUM_MAX-cnt;
      }
      bytes=__Widget_CharToBytes(str, num);
      //将字符串加在原来字符串后面
      for(i=0;i<bytes;i++)
      {
          *(text+len+i)=*(str+i);
      }
      *(text+len+bytes)='\0';
      return true;
}

// =============================================================================
// 函数功能:在TextBox指定位置开始删除字符串
// 输入参数: hwnd,文本框窗体句柄；
//          idx,字符编号；
//          count,字符数.
// 输出参数: 无。
// 返回值  :成功则返回true，失败则返回false.
// =============================================================================
static bool_t __Widget_TextBoxDeleteChar(HWND hwnd,u8 idx,u8 count)
{
     char *text;
     char temp_str[2*CN_CHAR_NUM_MAX+1];
     u8 i,cnt,k,tmp;
     u16 f_len,len,last_len=0;
     s16 str_len=0;
//     bool_t ret;
     TextBox *pTB;
     if(hwnd==NULL)
         return false;
     pTB=(TextBox *)GDD_GetWindowPrivateData(hwnd);
     if(pTB==NULL)
         return false;
     if(pTB->EditProperty==WS_TEXTBOX_R_O)
         return false;
     text=hwnd->Text;
     cnt=pTB->ChNum;
     str_len=__Widget_GetValidStrLen(Charset_NlsGetCurCharset(), text);
     if(str_len==-1)
         return false;
     if(count>idx)
         return false;
     tmp=idx-count;
     if(tmp==0)
     {
         k=cnt-idx;
         if(k==0)
         {
            *text='\0';
            return true;
         }
         else
         {
             len=__Widget_CharToBytes(text,idx);
             last_len=str_len-len;
             for(i=0;i<last_len;i++)
             {
                temp_str[i]=*(text+len+i);
             }
             temp_str[last_len]='\0';
             str_len=last_len+1;
             memcpy(text,temp_str,str_len);
             return true;
         }
     }

     f_len=__Widget_CharToBytes(text,tmp);
     for(i=0;i<f_len;i++)
     {
        temp_str[i]=*(text+i);
     }
     if(idx!=cnt)
     {
        len=__Widget_CharToBytes(text,idx);
        last_len=str_len-len;
        for(i=0;i<last_len;i++)
        {
            temp_str[f_len+i]=*(text+len+i);
        }
     }
     temp_str[f_len+last_len]='\0';
     str_len=f_len+last_len+1;
     memcpy(text,temp_str,str_len);
     return true;
}

// =============================================================================
// 函数功能:在TextBox指定字符处开始插入字符串
// 输入参数:  hwnd,文本框窗体句柄；
//           idx,字符编号；
//           str,字符串指针.
// 输出参数: 无。
// 返回值  :成功则返回true，失败则返回false.
// =============================================================================
static bool_t __Widget_TextBoxInsertChar(HWND hwnd,u8 idx,char *str)
{
     char temp_str[2*CN_CHAR_NUM_MAX+1];
     char *text;
     u8 i;
     u16 f_len,last_len;
     u16 num=0,cnt;
     s16 bytes,str_len=0;
     TextBox *pTB;
     bool_t ret;
     if(hwnd==NULL)
        return false;
     if(str==NULL)
        return false;
     pTB=(TextBox *)GDD_GetWindowPrivateData(hwnd);
     if(pTB==NULL)
        return false;
     //检查文本框编辑属性
     if(pTB->EditProperty==WS_TEXTBOX_R_O)
        return false;
     text=hwnd->Text;
     str_len=__Widget_GetValidStrLen(Charset_NlsGetCurCharset(), text);
     if(str_len==-1)
        return false;
     ret=__Widget_GetValidStrInfo(str,&num,NULL);
     if(!ret)
         return false;
    //字符串原有的字符数
     cnt=pTB->ChNum;
     if(cnt+num>CN_CHAR_NUM_MAX)
     {
          num=CN_CHAR_NUM_MAX-cnt;
     }
     bytes=__Widget_CharToBytes(str, num);
     f_len=__Widget_CharToBytes(text,idx);

     for(i=0;i<f_len;i++)
     {
         temp_str[i]=*(text+i);
     }
     //Insert
     for(i=0;i<bytes;i++)
     {
         temp_str[f_len+i]=*(str+i);
     }
     last_len=str_len-f_len;
     for(i=0;i<last_len;i++)
     {
         temp_str[f_len+bytes+i]=*(text+f_len+i);
     }
     temp_str[f_len+bytes+last_len]='\0';
     str_len=f_len+bytes+last_len+1;
     memcpy(text,temp_str,str_len);

     return true;
}

// =============================================================================
// 函数功能: 文本框控件Create消息处理函数.
// 输入参数: pMsg,窗体消息指针.
// 输出参数: 无。
// 返回值  :成功则返回true，失败则返回false.
// =============================================================================
static bool_t __Widget_TextBoxCreate(struct WindowMsg *pMsg)
{
    HWND hwnd;
    TextBox *pTB;
    u32 Style=0;

    printf("TextBox_Create\r\n");

    if(pMsg==NULL)
        return false;
    hwnd =pMsg->hwnd;
    if(hwnd==NULL)
        return false;
    hwnd =pMsg->hwnd;
    Style=hwnd->Style;
    if(pMsg->Param1==0)
    {
        pTB=(TextBox *)malloc(sizeof(TextBox));
        if(pTB==NULL)
            return false;
        memset(pTB, 0, sizeof(TextBox));
        pTB->ChNum=0;
        pTB->CharNumLimit=CN_CHAR_NUM_MAX;
        if(Style & WS_TEXTBOX_R_O)
        {
            pTB->EditProperty=WS_TEXTBOX_R_O;
        }
        else if(Style & WS_TEXTBOX_W_O)
        {
            pTB->EditProperty=WS_TEXTBOX_W_O;
        }
        else
        {
            pTB->EditProperty=WS_TEXTBOX_R_W;
        }
        pTB->Visible=true;
        pTB->CursorLoc=0;
        pTB->IsMultiLines=false;
        pTB->MaxLines=1;
        pTB->CharWidthSum=0;
        GDD_SetWindowPrivateData(hwnd,(ptu32_t)pTB);
    }

    return true;
}
// =============================================================================
// 函数功能: 判断按键按下的字符是否为合法ASCII字符.
// 输入参数: keyval:字符ASCII码.
// 输出参数: 无。
// 返回值  :是则返回true,否则返回false.
// =============================================================================
static bool_t __Widget_IsValidInputKey(u8 keyval)
{
    bool_t ret=true;
//    if(keyval<VK_NUM_0-1)
//    {
//        if(keyval!=VK_DECIMAL_POINT)
//            return false;
//    }
//    if(keyval>VK_NUM_9+1)
//        return false;
    if(keyval < VK_CHAR_START || keyval > VK_CHAR_END)
        return false;
    return ret;
}
// =============================================================================
// 函数功能: 文本框控件绘制函数
// 输入参数: pMsg,窗体消息指针
// 输出参数: 无。
// 返回值  :成功则返回true，失败则返回false.
// =============================================================================
static  bool_t __Widget_TextBoxPaint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC hdc;
    RECT rc;
    char *str="";
    u16 count;
    TextBox *pTB;
    bool_t ret;
    uint32_t color;
    if(pMsg==NULL)
        return false;
    hwnd =pMsg->hwnd;
    if(hwnd==NULL)
        return false;
    pTB=(TextBox *)GDD_GetWindowPrivateData(hwnd);
    if(pTB==NULL)
        return false;
    hdc =GDD_BeginPaint(hwnd);
    if(NULL==hdc)
        return false;
    //由于TextBox只能显示单行信息,因此先判断一下字符串是否包含多行,若为多行,则
    //只显示第一行信息,其他行信息直接忽略.
    if(!pTB->IsMultiLines)
    {
        str=hwnd->Text;
        ret=__Widget_GetValidStrInfo(str,&count,NULL);
        if(!ret)
            return false;
    }


   GDD_GetClientRect(hwnd,&rc);

   Widget_GetAttr(hwnd,ENUM_WIDGET_FILL_COLOR,&color);
//   GDD_SetFillColor(hdc,color);
   GDD_SetFillColor(hdc,RGB(178,178,178));
   GDD_FillRect(hdc,&rc);
   if(hwnd->Style&WS_BORDER)
   {
      if(hwnd->Style&LABEL_BORDER_FIXED3D)
      {
          GDD_SetDrawColor(hdc,RGB(173,173,173));
          GDD_DrawLine(hdc,0,0,0,GDD_RectH(&rc)-1); //L
          GDD_SetDrawColor(hdc,RGB(92,92,92));
          GDD_DrawLine(hdc,0,GDD_RectH(&rc)-1,GDD_RectW(&rc)-1,GDD_RectH(&rc)-1); //D
          GDD_DrawLine(hdc,GDD_RectW(&rc)-1,GDD_RectH(&rc),GDD_RectW(&rc)-1,0); //R
          GDD_SetDrawColor(hdc,RGB(173,173,173));
          GDD_DrawLine(hdc,GDD_RectW(&rc)-1,0,0,0);
      }
      else
      {
         GDD_SetDrawColor(hdc,RGB(169,169,169));
         GDD_DrawLine(hdc,0,0,0,GDD_RectH(&rc)-1); //L
         GDD_DrawLine(hdc,0,0,GDD_RectW(&rc)-1,0);   //U
         GDD_DrawLine(hdc,GDD_RectW(&rc)-1,0,GDD_RectW(&rc)-1,GDD_RectH(&rc)-1); //R
         GDD_DrawLine(hdc,0,GDD_RectH(&rc)-1,GDD_RectW(&rc)-1,GDD_RectH(&rc)-1); //D
      }
    }

    GDD_SetTextColor(hdc,RGB(28,32,42));
    Widget_GetAttr(hwnd,ENUM_WIDGET_TEXT_COLOR,&color);
    GDD_SetTextColor(hdc,color);
    GDD_DrawText(hdc,str,count,&rc,DT_VCENTER|DT_LEFT);
    GDD_EndPaint(hwnd,hdc);

    return true;
}

// =============================================================================
// 函数功能: TextBox控件KEY_DOWN_MSG消息响应函数
// 输入参数: pMsg,窗体消息结构体指针
// 输出参数: 无。
// 返回值  :成功返回true,失败则返回false。
// =============================================================================
static bool_t __Widget_TextBoxKeyDown(struct WindowMsg *pMsg)
{
    HWND hwnd;
    u8 cursorloc,chnum,chnummax,keyval;
    TextBox *pTB;
    char tmpbuf[2];
    bool_t ret;
    struct FontObj* cur_font;
    struct Charset* cur_enc;
    u32 wc;
    s32 chwidth,width=0;
    u32 ch;
    char *str;
    RECT rc;
    if(pMsg==NULL)
        return false;
    hwnd =pMsg->hwnd;
    if(hwnd==NULL)
        return false;
    ret=GDD_IsFocusWindow(hwnd);
    if(!ret)
        return false;
    keyval=(u8)pMsg->Param1;
    tmpbuf[0]=(char)keyval;
    tmpbuf[1]='\0';
    pTB=(TextBox *)GDD_GetWindowPrivateData(hwnd);
    cursorloc=pTB->CursorLoc;
    chnum=pTB->ChNum;
    chnummax=pTB->CharNumLimit;
    ret=__Widget_IsValidInputKey( keyval);
    if(ret)
    {
         if(chnum!=chnummax)
         {
            cur_font = Font_GetCurFont();
            cur_enc = Charset_NlsGetCurCharset();
            cur_enc->MbToUcs4(&wc, tmpbuf, -1);
            chwidth=cur_font->GetCharWidth(wc);
            GDD_GetClientRect(hwnd,&rc);
            width=GDD_RectW(&rc);
            if(width>=pTB->CharWidthSum+chwidth)
            {
                if(chnum!=cursorloc)
                {
                    __Widget_TextBoxInsertChar(hwnd, cursorloc, tmpbuf);
                }
                else
                {
                    __Widget_TextBoxAddChar(hwnd, tmpbuf);
                }
                cur_font = Font_GetCurFont();
                cur_enc = Charset_NlsGetCurCharset();
                cur_enc->MbToUcs4(&wc, tmpbuf, -1);
                chwidth=cur_font->GetCharWidth(wc);
                pTB->ChNum++;
                pTB->CursorLoc++;
                pTB->CharWidthSum+=chwidth;
                GDD_InvalidateWindow( hwnd, true);
                __Widget_MoveCursor(hwnd,pTB->CursorLoc);
             }
         }
    }
    else
    {
         switch (keyval)
         {
               case VK_ENTER_CHAR:
               {
                   GDD_CursorSetHide( );
                   break;
               }
               case VK_DEL_CHAR:
               {
                   if(pTB->ChNum>=1)
                   {
                     if(cursorloc!=0)
                     {
                        str=hwnd->Text;
                        ch=__Widget_GetCharByIndex(str,pTB->CursorLoc);
                        cur_font = Font_GetCurFont();
                        chwidth=cur_font->GetCharWidth(ch);
                        __Widget_TextBoxDeleteChar(hwnd, cursorloc,1);
                        pTB->ChNum--;
                        pTB->CursorLoc--;
                        pTB->CharWidthSum-=chwidth;
                     }
                     GDD_InvalidateWindow( hwnd, true);
                     __Widget_MoveCursor(hwnd,pTB->CursorLoc);
                   }
                   break;
               }
               case VK_UP:
               case VK_DOWN:
                    break;

           default:
            break;

         }
    }

    GDD_SetWindowPrivateData( hwnd, (ptu32_t)pTB);
    return true;
}

//// =============================================================================
//// 函数功能: TextBox控件KEY_UP_MSG消息响应函数
//// 输入参数: pMsg,窗体消息结构体指针
//// 输出参数: 无。
//// 返回值  :无。
//// =============================================================================
//static bool_t TextBox_KeyUp(struct WindowMsg *pMsg)
//{
//   return true;
//}
//// =============================================================================
//// 函数功能: TextBox控件KEY_PRESS_MAG消息绘制函数
//// 输入参数: pMsg,窗体消息结构体指针
//// 输出参数: 无。
//// 返回值  :无。
//// =============================================================================
//static bool_t TextBox_KeyPress(struct WindowMsg *pMsg)
//{
//    return true;
//}
//
//
//
//static bool_t TextBoxL_Down(struct WindowMsg *pMsg)
//{
//    return true;
//}
//
//static bool_t TextBoxL_Up(struct WindowMsg *pMsg)
//{
//   return true;
//}
//
//
//static void TextBox_TouchUp(struct WindowMsg *pMsg)
//{
//
//}

// =============================================================================
// 函数功能: TextBox控件触摸屏按下响应函数。
// 输入参数: pMsg,窗体消息结构体指针
// 输出参数: 无。
// 返回值  :成功返回true,失败则返回false。
// =============================================================================
static bool_t __Widget_TextBoxTouchDown(struct WindowMsg *pMsg)
{
    HWND hwnd;
    u32 loc,Style;;
    u16 chnum,idx,CharWidth,x,y;
    TextBox *pTB;
    char *str;
    s32 tmp;
    RECT rc;

    if(pMsg==NULL)
        return false;
    hwnd =pMsg->hwnd;
    if(hwnd==NULL)
        return false;
    pTB=(TextBox *)GDD_GetWindowPrivateData(hwnd);
    if(pTB==NULL)
        return false;
    //不可编辑的文本框(Lable不响应该事件)
    Style=pTB->EditProperty;
    if(Style==WS_TEXTBOX_R_O)
          return false;
    str=hwnd->Text;
    if(str!=NULL)
    {
       __Widget_GetValidStrInfo(str,&chnum,&CharWidth);
    }
    pTB->ChNum=chnum;
    pTB->CharWidthSum=CharWidth;
    //在文本框上
    loc=pMsg->Param2;   //获取触摸点信息
    x=loc;
    y=loc>>16;
    chnum=pTB->ChNum;
    str=hwnd->Text;
    if(str==NULL)
    {
         pTB->CursorLoc=0;
        __Widget_MoveCursor(hwnd,0);
    }
    else
    {
        __Widget_GetValidStrInfo(str,NULL,&CharWidth);
        GDD_GetWindowRect(hwnd,&rc);
        tmp=rc.left+CharWidth;
        if(x>tmp)
        {
            pTB->CursorLoc=chnum;
            __Widget_MoveCursor(hwnd,chnum);
        }
        else
        {
           idx=__Widget_FindIdx(hwnd,x,y);
           pTB->CursorLoc=idx;
           __Widget_MoveCursor(hwnd,idx);
        }
    }
    GDD_SetWindowPrivateData(hwnd,(ptu32_t)pTB);

    GDD_CursorSetHost(hwnd);

    GDD_SetFocusWindow(hwnd);

    return true;
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// =============================================================================
// 函数功能: TextBox控件获得焦点消息响应函数。
// 输入参数: pMsg,窗体消息指针
// 输出参数: 无。
// 返回值  :成功返回true,失败则返回false。
// ======================================================================
static bool_t __Widget_TextBoxSetFocus(struct WindowMsg *pMsg)
{
//    HWND hwnd,Tmrhwnd;
//    if(pMsg==NULL)
//        return false;
//    hwnd =pMsg->hwnd;
//    if(hwnd==NULL)
//        return false;
//    Tmrhwnd=GDD_GetDesktopWindow(NULL);
//    if(Tmrhwnd!=NULL)
//    {
//       GDD_PostMessage(Tmrhwnd,MSG_TIMER_START,CN_CURSOR_TIMER_ID,(ptu32_t)hwnd);
//    }
    return true;
}
// =============================================================================
// 函数功能: TextBox控件失去焦点消息响应函数。
// 输入参数: pMsg,窗体消息指针
// 输出参数: 无。
// 返回值  :成功返回true,失败则返回false。
// ======================================================================
static bool_t __Widget_TextBoxKillFocus(struct WindowMsg *pMsg)
{
//     HWND hwnd,Tmrhwnd;
//     if(pMsg==NULL)
//          return false;
//     hwnd =pMsg->hwnd;
//     if(hwnd==NULL)
//          return false;
//     Tmrhwnd=GDD_GetDesktopWindow(NULL);
//     if(Tmrhwnd!=NULL)
//     {
//         GDD_PostMessage(Tmrhwnd,MSG_TIMER_STOP,CN_CURSOR_TIMER_ID,(ptu32_t)hwnd);
//     }
     return true;
}
#pragma GCC diagnostic pop

// =============================================================================
// 函数功能: TextBox控件获取文本内容函数。
// 输入参数: hwnd,文本框窗口句柄.
// 输出参数: 无。
// 返回值  :无。
// =============================================================================
static void __Widget_TextBoxGetText(HWND hwnd,char *text)
{
     TextBox *pTB;
     u16 len;
     char *str;
     pTB = (TextBox *)GDD_GetWindowPrivateData(hwnd);
     if(pTB==NULL)
         return ;
     str=hwnd->Text;
     len=strlen(str);
     memcpy(text,str,len);
     *(text+len)='\0';
}

// =============================================================================
// 函数功能: TextBox控件设置文本Text函数。
// 输入参数: hwnd,文本框窗口句柄.
// 输出参数: 无。
// 返回值  :无。
// =============================================================================
static void __Widget_TextBoxSetText(HWND hwnd,char *str)
{
     u16 len;
     char *dst;
     dst=hwnd->Text;
     len=strlen(str);
     memcpy(dst,str,len);
     *(dst+len)='\0';
     GDD_InvalidateWindow(hwnd, true);
}

// =============================================================================
// 函数功能: TextBox控件删除文本函数。
// 输入参数: hwnd,文本框窗口句柄.
// 输出参数: 无。
// 返回值  :无。
// =============================================================================
static void __Widget_TextBoxDeleteText(HWND hwnd)
{
     hwnd->Text='\0';
     GDD_InvalidateWindow(hwnd, true);
}
// =============================================================================
// 函数功能: TextBox控件显示内容控制函数
// 输入参数: hwnd,TextBox控件窗口句柄;
//          ctrlcmd,详情参见enum TextCtrlCmd
//          para1:对于EN_GET_TEXT、EN_SET_TEXT、EN_DELETE_TEXT该参数无效,可直接置0。
//          para2:对于EN_DELETE_TEXT该参数无效,可直接置0,对于EN_GET_TEXT、EN_SET_TEXT
//          输入字符串指针.
// 返回值  :成功则返回true，失败则返回false.
// =============================================================================
bool_t Widget_TextBoxTextCtrl(HWND hwnd,u8 ctrlcmd,ptu32_t para1)
{
    if(hwnd==NULL)
        return false;
    switch(ctrlcmd)
    {
        case EN_GET_TEXT:
            __Widget_TextBoxGetText(hwnd,(char *)para1);
            break;
        case EN_SET_TEXT:
            __Widget_TextBoxSetText(hwnd,(char *)para1);
            break;
        case EN_DELETE_TEXT:
            __Widget_TextBoxDeleteText(hwnd);
            break;
        default:
            break;
    }
    return true;
}
bool_t TextBox_Close(HWND hwnd)
{
    TextBox *pTB;
    pTB = (TextBox *)GDD_GetWindowPrivateData(hwnd);
    if(pTB != NULL)
    {
        printf("this is TextBox_Close\r\n");
        free(pTB);
    }
    return true;
}

//默认按钮消息处理函数表，处理用户函数表中没有处理的消息。
static struct MsgProcTable s_gTextBoxMsgProcTable[] =
{
    {MSG_KEY_DOWN,__Widget_TextBoxKeyDown},
    {MSG_PAINT,__Widget_TextBoxPaint},
    {MSG_CREATE,__Widget_TextBoxCreate},
    {MSG_TOUCH_DOWN,__Widget_TextBoxTouchDown},
    {MSG_SETFOCUS,__Widget_TextBoxSetFocus},
    {MSG_KILLFOCUS,__Widget_TextBoxKillFocus}
};

static struct MsgTableLink  s_gTextBoxMsgLink;
// =============================================================================
// 函数功能: TextBox控件创建函数。
// 输入参数: Text:文本框窗口Text;
//           Style:文本框风格，参见gdd.h;
//           x:文本框起始位置x方向坐标(单位：像素);
//           y:文本框起始位置y方向坐标(单位：像素);
//           w:文本框宽度(单位：像素);
//           h:文本框高度(单位：像素);
//           hParent:文本框父窗口句柄;
//           WinId:文本框控件Id;
//           pdata:文本框控件私有数据结构;
//           UserMsgTableLink:文本框控件用户消息列表结构指针。
// 输出参数: 无。
// 返回值  :成功则返回文本框句柄，失败则返回NULL。
// =============================================================================
HWND Widget_CreateTextBox(const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,TextBox *pTB,
                    struct MsgTableLink *UserMsgTableLink)
{
    HWND pGddWin;
    if(hParent == NULL)
        hParent = GDD_GetDesktopWindow(NULL);
    //加锁后，GDD_GetMessage函数将不能立即取出消息，确保 GDD_AddProcFuncTable 函数
    //完成后，即消息处理函数表完整后再取出消息处理。
    if(__HWND_Lock(hParent))
    {
        s_gTextBoxMsgLink.MsgNum = sizeof(s_gTextBoxMsgProcTable) / sizeof(struct MsgProcTable);
        s_gTextBoxMsgLink.myTable = (struct MsgProcTable *)&s_gTextBoxMsgProcTable;
        pGddWin = GDD_CreateWindow(Text,  WS_CAN_FOCUS | WS_SHOW_CURSOR | Style,
                                    x, y, w, h, hParent, WinId, CN_WINBUF_PARENT,
                                    (ptu32_t)pTB, CN_SYS_PF_DISPLAY, CN_COLOR_WHITE, &s_gTextBoxMsgLink);
        if(UserMsgTableLink != NULL)
            GDD_AddProcFuncTable(pGddWin,UserMsgTableLink);
        __HWND_Unlock(hParent);
        return pGddWin;
    }
    else
        return NULL;
}


