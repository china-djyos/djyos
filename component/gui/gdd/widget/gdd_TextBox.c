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
//����:  zhb.
//�汾��V1.0.0
//�ļ�����: ��ť�ؼ�ʵ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2016-6-14
//   ����:  zhb.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//---------------------------------

#include  "gdd.h"
#include  "../gdd_private.h"
#include  <font/font.h>
#include  <gdd_widget.h>


#define CN_CHAR_NUM_MAX                 255
#define CN_CANCLE_KEY                   0xA3

// =============================================================================
// ��������: ��ȡ�ַ��������ֽ���,TextBoxֻ����ʾ������Ϣ�����ȼ���ַ���,�������
//           \n(���з�)���������ȡ���з�֮ǰ���ַ���Ϊ��Ч���ַ���.
// �������: char *str:�ַ���
// �������: �ޡ�
// ����ֵ  :��ȡ����Ч�ַ��ֽ�����
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
                *(str+1)='\0';  //todo: �����const����ô�죿
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
// ��������: ��ȡ�ַ�����ָ����ŵ��ַ�����Ŵ�1,2,3......��ʼ.
// �������: str:�ַ���ָ��
//           idx:�ַ����
// �������:
// ����ֵ  :ucs4�ַ�
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
     //�����ַ������ַ���
//    cur_font = Font_GetCurFont();
    for(; str_len > 0;)
    {
       len= cur_enc->MbToUcs4(&wc, str, -1);
       if(len == -1)
       {  // ��Ч�ַ�
           str_len--;
           str++;
       }
       else if(len == 0)
       {
            break;
       }
       else
       {    // ��Ч�ַ�
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
// ��������: ��ȡ�ַ������ַ������ܵ��ַ���ռ��������
// �������: str:�ַ���ָ��
//          pChunm:�ַ���ָ��,�������ΪNULLʱ�򲻼����ַ�����
//          pChWidthSum:�ַ���ռ��������ָ�룬�������ΪNULLʱ�򲻼����ַ���ռ�����ؿ��.
// �������: �ޡ�
// ����ֵ  :�ɹ��򷵻�true��ʧ���򷵻�false.
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
    cur_enc = Charset_NlsGetCurCharset();//��ȡ��ǰ�ַ���
    str_len=__Widget_GetValidStrLen(cur_enc, str);//��ȡ�ַ��������ֽ���
    if(str_len==-1)
        return false;
     //�����ַ������ַ���
     cur_font = Font_GetCurFont();//��ȡ����
     for(; str_len > 0;)
     {
        len= cur_enc->MbToUcs4(&wc, str, -1);
        if(len == -1)
        {   // ��Ч�ַ�
            str_len--;
            str++;
        }
        else if(len == 0)
        {
             break;
        }
        else
        {          // ��Ч�ַ�
            str += len;
            str_len -= len;
            if(pChunm!=NULL)
            {
              cnt++;
            }
            if(pChWidthSum!=NULL)
            {
              chwidth=cur_font->GetCharWidth(wc);//�������д�����ռ�������ж���

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
// ��������: ��ȡ�ַ�����ָ������ַ���ռ�����ؿ��.
// �������: str:�ַ���ָ��
//          idx:�ַ����.
// �������: �ޡ�
// ����ֵ  :�ַ������ܿ��.
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
    str_len=__Widget_GetValidStrLen(cur_enc, str);//�����ַ������ַ���
    if(str_len==-1)
         return -1;
    if(idx==0)
        return 0;
    //�����ַ������ַ���
    cur_font = Font_GetCurFont();//����
    for(; str_len > 0;)
    {
        len= cur_enc->MbToUcs4(&wc, str, -1);
        if(len == -1)
        {   // ��Ч�ַ�
            str_len--;
            str++;
        }
        else if(len == 0)
             break;
        else
        {          // ��Ч�ַ�
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
// ��������: �ƶ����.
// �������: hwnd:�ı��򴰿ھ��;
//          idx:�ı����ַ����.
// �������: �ޡ�
// ����ֵ  :�ɹ��򷵻�true��ʧ���򷵻�false.
// =============================================================================
static bool_t __Widget_MoveCursor(HWND hwnd,u8 idx)
{
    //�ڲ����ã�������в������
    TextBox *pTB;
    HDC hdc;
    char *str;
    RECT rc,rc0;
    u16 width;
    s32 x,y;
    pTB=(TextBox *)GDD_GetWindowPrivateData(hwnd);//���˽������
    if(pTB==NULL)
        return false;
    GDD_GetClientRectToScreen(hwnd,&rc);//ת������
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
// ��������: ���ݴ����������ȡ�ı������ַ����
// �������: hwnd:�ı��򴰿ھ��
//           x:������x��������
//           y:������y��������
// �������: �ޡ�
// ����ֵ  :�ı������ַ����.
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
// ��������: ��ȡ�ַ����дӿ�ʼ��ָ����ŵ��ַ��ܵ��ֽ���.
// �������: str:�ַ���ָ��
//           idx:�ַ�����ָ���ַ���ţ���1��ʼ��������������.
// �������: ��
// ����ֵ  :ָ���ַ���ַ.
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
        {     // ��Ч�ַ�
            str_len--;
            str++;
            bytes++;
        }
        else if(len == 0)
        {
            break;
        }
        else
        {          // ��Ч�ַ�
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
// ��������:��TextBox������ַ���
// �������: hwnd:�ı�������
//          str:�ַ���ָ��
// �������: �ޡ�
// ����ֵ  :�ɹ��򷵻�true��ʧ���򷵻�false.
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
    //����ı���༭����
     if(pTB->EditProperty==WS_TEXTBOX_R_O)
        return false;
     text=hwnd->Text;
     len=__Widget_GetValidStrLen(Charset_NlsGetCurCharset(), text);
     if(len==-1)
          return false;
     //���һ��str�ĺϷ���
      str_len=__Widget_GetValidStrLen(Charset_NlsGetCurCharset(), str);
      if(str_len==-1)
          return false;
      ret=__Widget_GetValidStrInfo(str,&num,NULL);
      if(!ret)
          return false;
      //�ַ���ԭ�е��ַ���
      cnt=pTB->ChNum;
      if(cnt+num>CN_CHAR_NUM_MAX)
      {
           num=CN_CHAR_NUM_MAX-cnt;
      }
      bytes=__Widget_CharToBytes(str, num);
      //���ַ�������ԭ���ַ�������
      for(i=0;i<bytes;i++)
      {
          *(text+len+i)=*(str+i);
      }
      *(text+len+bytes)='\0';
      return true;
}

// =============================================================================
// ��������:��TextBoxָ��λ�ÿ�ʼɾ���ַ���
// �������: hwnd,�ı���������
//          idx,�ַ���ţ�
//          count,�ַ���.
// �������: �ޡ�
// ����ֵ  :�ɹ��򷵻�true��ʧ���򷵻�false.
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
// ��������:��TextBoxָ���ַ�����ʼ�����ַ���
// �������:  hwnd,�ı���������
//           idx,�ַ���ţ�
//           str,�ַ���ָ��.
// �������: �ޡ�
// ����ֵ  :�ɹ��򷵻�true��ʧ���򷵻�false.
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
     //����ı���༭����
     if(pTB->EditProperty==WS_TEXTBOX_R_O)
        return false;
     text=hwnd->Text;
     str_len=__Widget_GetValidStrLen(Charset_NlsGetCurCharset(), text);
     if(str_len==-1)
        return false;
     ret=__Widget_GetValidStrInfo(str,&num,NULL);
     if(!ret)
         return false;
    //�ַ���ԭ�е��ַ���
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
// ��������: �ı���ؼ�Create��Ϣ������.
// �������: pMsg,������Ϣָ��.
// �������: �ޡ�
// ����ֵ  :�ɹ��򷵻�true��ʧ���򷵻�false.
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
// ��������: �жϰ������µ��ַ��Ƿ�Ϊ�Ϸ�ASCII�ַ�.
// �������: keyval:�ַ�ASCII��.
// �������: �ޡ�
// ����ֵ  :���򷵻�true,���򷵻�false.
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
// ��������: �ı���ؼ����ƺ���
// �������: pMsg,������Ϣָ��
// �������: �ޡ�
// ����ֵ  :�ɹ��򷵻�true��ʧ���򷵻�false.
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
    //����TextBoxֻ����ʾ������Ϣ,������ж�һ���ַ����Ƿ��������,��Ϊ����,��
    //ֻ��ʾ��һ����Ϣ,��������Ϣֱ�Ӻ���.
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
// ��������: TextBox�ؼ�KEY_DOWN_MSG��Ϣ��Ӧ����
// �������: pMsg,������Ϣ�ṹ��ָ��
// �������: �ޡ�
// ����ֵ  :�ɹ�����true,ʧ���򷵻�false��
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
//// ��������: TextBox�ؼ�KEY_UP_MSG��Ϣ��Ӧ����
//// �������: pMsg,������Ϣ�ṹ��ָ��
//// �������: �ޡ�
//// ����ֵ  :�ޡ�
//// =============================================================================
//static bool_t TextBox_KeyUp(struct WindowMsg *pMsg)
//{
//   return true;
//}
//// =============================================================================
//// ��������: TextBox�ؼ�KEY_PRESS_MAG��Ϣ���ƺ���
//// �������: pMsg,������Ϣ�ṹ��ָ��
//// �������: �ޡ�
//// ����ֵ  :�ޡ�
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
// ��������: TextBox�ؼ�������������Ӧ������
// �������: pMsg,������Ϣ�ṹ��ָ��
// �������: �ޡ�
// ����ֵ  :�ɹ�����true,ʧ���򷵻�false��
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
    //���ɱ༭���ı���(Lable����Ӧ���¼�)
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
    //���ı�����
    loc=pMsg->Param2;   //��ȡ��������Ϣ
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
// ��������: TextBox�ؼ���ý�����Ϣ��Ӧ������
// �������: pMsg,������Ϣָ��
// �������: �ޡ�
// ����ֵ  :�ɹ�����true,ʧ���򷵻�false��
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
// ��������: TextBox�ؼ�ʧȥ������Ϣ��Ӧ������
// �������: pMsg,������Ϣָ��
// �������: �ޡ�
// ����ֵ  :�ɹ�����true,ʧ���򷵻�false��
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
// ��������: TextBox�ؼ���ȡ�ı����ݺ�����
// �������: hwnd,�ı��򴰿ھ��.
// �������: �ޡ�
// ����ֵ  :�ޡ�
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
// ��������: TextBox�ؼ������ı�Text������
// �������: hwnd,�ı��򴰿ھ��.
// �������: �ޡ�
// ����ֵ  :�ޡ�
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
// ��������: TextBox�ؼ�ɾ���ı�������
// �������: hwnd,�ı��򴰿ھ��.
// �������: �ޡ�
// ����ֵ  :�ޡ�
// =============================================================================
static void __Widget_TextBoxDeleteText(HWND hwnd)
{
     hwnd->Text='\0';
     GDD_InvalidateWindow(hwnd, true);
}
// =============================================================================
// ��������: TextBox�ؼ���ʾ���ݿ��ƺ���
// �������: hwnd,TextBox�ؼ����ھ��;
//          ctrlcmd,����μ�enum TextCtrlCmd
//          para1:����EN_GET_TEXT��EN_SET_TEXT��EN_DELETE_TEXT�ò�����Ч,��ֱ����0��
//          para2:����EN_DELETE_TEXT�ò�����Ч,��ֱ����0,����EN_GET_TEXT��EN_SET_TEXT
//          �����ַ���ָ��.
// ����ֵ  :�ɹ��򷵻�true��ʧ���򷵻�false.
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

//Ĭ�ϰ�ť��Ϣ�������������û���������û�д������Ϣ��
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
// ��������: TextBox�ؼ�����������
// �������: Text:�ı��򴰿�Text;
//           Style:�ı����񣬲μ�gdd.h;
//           x:�ı�����ʼλ��x��������(��λ������);
//           y:�ı�����ʼλ��y��������(��λ������);
//           w:�ı�����(��λ������);
//           h:�ı���߶�(��λ������);
//           hParent:�ı��򸸴��ھ��;
//           WinId:�ı���ؼ�Id;
//           pdata:�ı���ؼ�˽�����ݽṹ;
//           UserMsgTableLink:�ı���ؼ��û���Ϣ�б�ṹָ�롣
// �������: �ޡ�
// ����ֵ  :�ɹ��򷵻��ı�������ʧ���򷵻�NULL��
// =============================================================================
HWND Widget_CreateTextBox(const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,TextBox *pTB,
                    struct MsgTableLink *UserMsgTableLink)
{
    HWND pGddWin;
    if(hParent == NULL)
        hParent = GDD_GetDesktopWindow(NULL);
    //������GDD_GetMessage��������������ȡ����Ϣ��ȷ�� GDD_AddProcFuncTable ����
    //��ɺ󣬼���Ϣ����������������ȡ����Ϣ����
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


