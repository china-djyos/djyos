#include <stdint.h>
#include <stddef.h>
#include <gkernel.h>
#include <gdd.h>
#include <gdd_widget.h>
#include <gui\gdd_cursor.h>

#include    <gui/gdd/gdd_private.h>

#include    <string.h>
#include    <_ansi.h>
/*============================================================================*/
#define ID_INSERT   0x1100
#define ID_COLOR    0x1102
#define CN_MAX_CHAR_ONE_LINE   128      //һ�����128���ַ�
#define CN_MAX_LINES           64       //���64��
#define ID_ENTER    0x1000
typedef struct
{
   u8 cur_byte_idx;       //��ǰ���ַ���;
   bool_t x_scrollbar;    //�Ƿ���ˮƽ���������
   bool_t y_scrollbar;    //�Ƿ�����ֱ���������
   u16 height;            //������
   u16 width;             //������
   u16 cur_line_index;    //��ǰ������
   u16 len;               //RichTextBox�������ַ����ֽ���
   char *pcurchar;        //��ǰָ����ַ�
   char *text;            //RichTextBox�����е��ַ���ɵ��ַ���
   u8 CursorLoc;          //�������idλ��
   u16 idwidth;           //��������ַ������������
   u16 idheight;           //��������ַ������ඥ���߶�
}RichTextBox;


static  s8 cfg_idx=0;
static  s8 color_idx=0;
static  s8 bBorder=FALSE;
static  s8 bBKGND=FALSE;
static  s8 bColor=FALSE;
//static  ptu32_t ButtonL_Up(struct WindowMsg *pMsg)
//

#include <stdio.h>

//�����ı�����ʾ��ԭ�ַ���
char *text="���֮�ѣ����������죡\n�ϴԼ����죬������ãȻ�����������ǧ�꣬��������ͨ���̡�";
char *add="�������\n������롣";      //���ǰ��°����������ַ���
char temp[300];                      //������ž����������ַ���
char temp_str[300];

// =============================================================================
// ��������: ��ȡ�ַ��������ֽ���,RichTextBox�ܹ���ʾ������Ϣ��
// �������: char *str:�ַ���
// �������: �ޡ�
// ����ֵ  :��ȡ����Ч�ַ��ֽ�����
// =============================================================================
static s16 __GetValidStrLen(char *str)
{
    u8 cnt=0;
    char ch;
    s16 str_len=0;
    if(str==NULL)
        return -1;
    str_len=strlen(str);
    return str_len;
}


// =============================================================================
// ��������: ��ȡ�ַ������ַ������ܵ��ַ���ռ��������
// �������: str:�ַ���ָ��
//          pChunm:�ַ���ָ��,�������ΪNULLʱ�򲻼����ַ�����
//          pChWidthSum:�ַ���ռ��������ָ�룬�������ΪNULLʱ�򲻼����ַ���ռ�����ؿ��.
// �������: �ޡ�
// ����ֵ  :�ɹ��򷵻�true��ʧ���򷵻�false.
// =============================================================================
static bool_t __GetValidStrInfo(char *str,u16 *pChunm,u16 *pChWidthSum)
{
    u16 cnt=0,chwidthsum=0;
    s16 str_len=0;
    s32 len,chwidth=0;
    struct FontRsc* cur_font;
    struct Charset* cur_enc;
    u32 wc;
    if(str==NULL)
        return false;
    str_len=__GetValidStrLen(str);
    if(str_len==-1)
        return false;
     //�����ַ������ַ���
     cur_font = Font_GetCurFont();
     cur_enc = Charset_NlsGetCurCharset();
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
            if(pChunm!=NULL)  //ͳ���м����ַ�������ʶ����ֽ��ַ���
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
// ��������: ��ȡ�ַ����дӿ�ʼ��ָ����ŵ��ַ��ܵ��ֽ���.
// �������: str:�ַ���ָ��
//           idx:�ַ�����ָ���ַ���ţ���1��ʼ��������������.
// �������: ��
// ����ֵ  :ָ���ַ���ַ.
// =============================================================================
static s16 __CharToBytes(char *str,u8 num)
{
     u16 bytes=0;
     s16 str_len=0;
     u8 cnt=0;
     u32 wc;
     s32 len;
     struct FontRsc* cur_font;
     struct Charset* cur_enc;
     if(str==NULL)
          return -1;
     str_len=__GetValidStrLen(str);
     if(str_len==-1)
        return -1;
     if(num==0)
         return -1;
     cur_font = Font_GetCurFont();
     cur_enc = Charset_NlsGetCurCharset();
     for(; str_len > 0;)
     {
        len= cur_enc->MbToUcs4(&wc, str, -1);//����ÿһ���ַ��ж೤ ���ۼ�����
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
// ��������:��RichTextBoxָ���ַ�����ʼ�����ַ���
// �������:  hwnd,�ı���������
//           idx,�ַ���ţ�
//           str,�ַ���ָ��.
// �������: �ޡ�
// ����ֵ  :�ɹ��򷵻�true��ʧ���򷵻�false.
// =============================================================================
static s32 RichTextBox_InsertChar(char *text,char *add,HWND hwnd)
{
	     u8 i,j;
	     RichTextBox *pRTB;
		 u8 num=0;
		 s16 str_len=0;
		 s16 len=0;
		 pRTB=hwnd->PrivateData;
	     str_len=__GetValidStrLen(text);
	     len=__CharToBytes(text,pRTB->CursorLoc);//�˴��޸��ַ�������λ��
	     if(len==-1)
	    	 len=0;

	     for(i=0,j=0;j<str_len;i++,j++)
	    	     {
	    	    	 if(i==len)
	    	    	 {
	    	    		while(1)
	    	    		{
	    	    			temp[i]=*(add+num);
	    	    		    i++;
	    	    		    num++;
	    	    		    if(*(add+num)=='\0')
	    	    		    	break;
	    	    		}


	    	    	 }
	    	    	 temp[i]=*(text+j);
	    	     }
	     temp[i]='\0';
		 return temp;

}


// =============================================================================
// ��������:��RichTextBox�У�����Ҫ����뻻�з�ʹ�ַ����ܹ�������ʾ
// �������:  hwnd,�ı���������
//           text,�ַ���ָ��.
// �������: �ޡ�
// ����ֵ  :���ؾ������������˻��з����ַ���ָ��
// =============================================================================


static s32 RichTextBox_LineFeed(char *text,HWND hwnd)
{
	     u8 i,j;
	     s8 num=0;
	     s16 str_len=0;
	     RichTextBox *pRTB;
	     pRTB=hwnd->PrivateData;


	     str_len=__GetValidStrLen(text);

	     for(i=0,j=0;j<str_len;i++,j++,num++)
	     {
	    	 if(*(text+j)=='\n')
	    	 {
	    		 num=-1;
	    	 }
	    	 if(num==pRTB->cur_byte_idx)
	    	 {
	    		 temp_str[i]='\n';
	    		 i++;
	    		 num=0;
	    	 }
	    	 temp_str[i]=*(text+j);
	     }
	     temp_str[i]='\0';
	     return temp_str;


}

// =============================================================================
// ��������: ��ȡ�ַ�����ָ������ַ���ռ�����ؿ��.
// �������: str:�ַ���ָ��
//          idx:�ַ����.
// �������: �ޡ�
// ����ֵ  :�ַ������ܿ��.
// =============================================================================
static s16 __GetStrWidth(char *str,u16 idx)
{
    struct FontRsc* cur_font;
    struct Charset* cur_enc;
    u32 wc;
    s32 len,chwidth=0;
    u16 cnt=0,chwidthsum=0;
    s16 str_len=0;
    if(str==NULL)
         return -1;
    str_len=__GetValidStrLen(str);
    if(str_len==-1)
         return -1;
    if(idx==0)
        return 0;
    //�����ַ������ַ���
    cur_font = Font_GetCurFont();
    cur_enc = Charset_NlsGetCurCharset();
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
static bool_t __MoveCursor(HWND hwnd,u8 idx)
{
    //�ڲ����ã�������в������
    RichTextBox *pRTB;
    char *str;
    RECT rc;
    u16 width;
    s32 x,y;
    pRTB=(RichTextBox *)GetWindowPrivateData(hwnd);
    if(pRTB==NULL)
        return false;
    GetClientRectToScreen(hwnd,&rc);
    str=text;
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
    Cursor_Move(x,y);

//    MoveWindow(g_CursorHwnd,x,y);
    UpdateDisplay(CN_TIMEOUT_FOREVER);
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
static s16 __FindIdx(HWND hwnd,u16 x,u16 y)
{
    char *str;
    RichTextBox *pRTB;


    //�����������趨��
    struct FontRsc* cur_font;
    s32 count,id=0;
    s32 linenum=0;
    s32 numinline[10];
    const char *line=text;
    char *linenext;


    RECT rc;
    s16 tmp=0,val1=0,val2=0;
    u16 chnum,idx,width=0;
    s32 line_count,line_h,y0;


    str=text;
    if(str==NULL)
        return -1;
    pRTB=(RichTextBox *)GetWindowPrivateData(hwnd);

//����ÿ�и߶�
    cur_font = Font_GetCurFont();
    line_h = Font_GetFontLineHeight(cur_font);

//������������
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
                	numinline[linenum]=count+1;
                    linenum++;
                break;
            }
        }



//���������ڵڼ���       (Ĭ������룬ˮƽ����)
         GetClientRectToScreen(hwnd,&rc);
         y0 =rc.top;
         y0 += ((RectH(&rc)-(linenum*line_h)-((linenum-1)*line_h))/2)-(0.5*line_h);//��ʼ�߶ȶ��ڵ�һ�����ư��еĵط�
         for(line_count=1;line_count<=linenum;line_count++)
         {
        	 tmp=y0+(line_count*2)*line_h;//�����м�࣬ÿ�и߶����������ַ��߶� ����һ�У����¼�������
        	 if(tmp>y)
        	 {
        		  break;
        	 }
        	 id+=numinline[line_count-1];
         }
         pRTB->idheight=y0+(0.5*line_h)+(line_count-1)*2*line_h;



 //ͨ�����ؿ�ȼ�����������һ�еĵڼ�����
    if(pRTB==NULL)
        return -1;
    __GetValidStrInfo(str,&chnum,NULL);

    for(idx=id;idx<(id+numinline[line_count-1]);idx++)
    {
        width=__GetStrWidth(str, idx)-__GetStrWidth(str, id);
        tmp=rc.left+width;
        if(tmp>x)
        {
           break;
        }
    }
    val1=tmp-x;
    width=__GetStrWidth(str, idx-1)-__GetStrWidth(str, id);
    tmp=rc.left+width;
    val2=x-tmp;
    if(val1>val2)
    {
    	pRTB->idwidth=__GetStrWidth(str, idx-1)-__GetStrWidth(str, id);
        return idx-1;
    }
    else
    {
    	pRTB->idwidth=__GetStrWidth(str, idx)-__GetStrWidth(str, id);
        return idx;
    }
}


// =============================================================================
// ��������: RichTextBox�ؼ�������������Ӧ��������ӦMSG_TOUCH_DOWN��Ϣ
// �������: pMsg,������Ϣ�ṹ��ָ��
// �������: �ޡ�
// ����ֵ  :�ɹ�����true,ʧ���򷵻�false��
// =============================================================================
static bool_t RichTextBox_TouchDown(struct WindowMsg *pMsg)
{
    HWND hwnd;
    u32 loc,Style;;
    u16 chnum,idx,CharWidth,x,y;
    RichTextBox *pRTB;
    char *str;
    s32 tmp;
    RECT rc;
    bool_t ret;

    str=text;
    hwnd =pMsg->hwnd;

    pRTB=(RichTextBox *)GetWindowPrivateData(hwnd);
    if(pRTB==NULL)
        return false;
    //���ɱ༭���ı���(Lable����Ӧ���¼�)

    __GetValidStrInfo(str,&chnum,&CharWidth);
    //���ı�����
    loc=pMsg->Param2;   //��ȡ��������Ϣ
    x=loc;
    y=loc>>16;
    if(str==NULL)
    {
         pRTB->CursorLoc=0;
        __MoveCursor(hwnd,0);
    }
    else
    {
        ret=__GetValidStrInfo(str,NULL,&CharWidth);
        GetWindowRect(hwnd,&rc);
        tmp=rc.left+CharWidth;
        if(x>tmp)
        {
            pRTB->CursorLoc=chnum;
            __MoveCursor(hwnd,chnum);
        }
        else
        {
           idx=__FindIdx(hwnd,x,y);
           pRTB->CursorLoc=idx;
           __MoveCursor(hwnd,idx);
        }
    }
    SetWindowPrivateData(hwnd,(void *)pRTB);

    Cursor_SetHost(hwnd);

    SetFocusWindow(hwnd);

    return true;
}

// =============================================================================
// ��������: RichTextBox�ؼ��ӿؼ����͵�֪ͨ����ӦMSG_NOTIFY��Ϣ
// �������: pMsg,������Ϣ�ṹ��ָ��
// �������: �ޡ�
// ����ֵ  :�ɹ�����true,ʧ���򷵻�false��
// =============================================================================
static ptu32_t RichTextBox_Notify(struct WindowMsg *pMsg)
{
        HWND hwnd;
        u16 event,id;

        hwnd =pMsg->hwnd;

        event =HI16(pMsg->Param1);
        id =LO16(pMsg->Param1);

        switch (id)
        {
        case ID_INSERT://����
        	 if(event==CBN_SELECTED)
        	 {
        	text=RichTextBox_InsertChar(text,add,hwnd);
            InvalidateWindow(hwnd,FALSE);
        	 }
            break;

        case ID_COLOR://�ı���ɫ
            if(event==CBN_SELECTED)
                {
                    bColor =TRUE;
                }
            if(event==CBN_UNSELECTED)
                {
                    bColor =FALSE;
                }
            InvalidateWindow(hwnd,true);
            break;
            default:break;
        }
    return true;
}

//����������
// =============================================================================
// ��������: RichTextBox�ؼ�������������ӦMSG_CREATE��Ϣ
// �������: pMsg,������Ϣ�ṹ��ָ��
// �������: �ޡ�
// ����ֵ  :�ɹ�����true,ʧ���򷵻�false��
// =============================================================================
static ptu32_t RichTextBox_Create(struct WindowMsg *pMsg)
{
    HWND hwnd;
    RECT rc0;
    u32 i;
    struct WinTimer *timer;
    RichTextBox *pRTB;
    hwnd =pMsg->hwnd;
    cfg_idx =0;

    if(pMsg->Param1==0)
  	    {
  	        pRTB=(RichTextBox *)malloc(sizeof(RichTextBox));
  	        if(pRTB==NULL)
  	            return false;

  	        pRTB->cur_byte_idx=16;//�˴��޸�ÿһ�ŵ��ַ���
  	        SetWindowPrivateData(hwnd,(void *)pRTB);
  	    }
    GetClientRect(hwnd,&rc0);
//    Cursor_Init();
    CreateButton("���뻻��",WS_CHILD|BS_NORMAL|WS_BORDER|WS_VISIBLE,RectW(&rc0)-64,RectH(&rc0)-60,60,24,hwnd,ID_INSERT,NULL,NULL);
    CreateButton("�ı���ɫ",WS_CHILD|BS_HOLD|WS_BORDER|WS_VISIBLE,RectW(&rc0)-64,RectH(&rc0)-30,60,24,hwnd,ID_COLOR,NULL,NULL);
    return true;
}

//���ڱ�������
static ptu32_t RichTextBox_Erasebkgnd(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC  hdc;
    RECT rc0;

    hwnd =pMsg->hwnd;
    hdc =(HDC)pMsg->Param1;
    GetClientRect(hwnd,&rc0);
    SetFillColor(hdc,RGB(200,200,200));
    FillRect(hdc,&rc0);

    return true;
}

//��ͼ��Ϣ
// =============================================================================
// ��������: RichTextBox�ؼ����ƺ�������ӦMMSG_PAINT��Ϣ
// �������: pMsg,������Ϣָ��
// �������: �ޡ�
// ����ֵ  :�ɹ��򷵻�true��ʧ���򷵻�false.
// =============================================================================
static ptu32_t RichTextBox_Paint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC  hdc;
    RECT rc0,rc;
    u32 i;
    hwnd =pMsg->hwnd;

    hdc = BeginPaint(hwnd);
    GetClientRect(hwnd,&rc0);

    SetRect(&rc,2,2,RectW(&rc0)-2*2,RectH(&rc0)-10);

    if(bColor==true)
       {
    	SetFillColor(hdc,RGB(200,200,0));
       }
    else
    	SetFillColor(hdc,RGB(0,200,0));

    FillRect(hdc,&rc);

    SetTextColor(hdc,RGB(1,0,0));
    SetDrawColor(hdc,RGB(255,255,255));
//    SetFillColor(hdc,RGB(200,0,240));

    i=DT_LEFT|DT_VCENTER;
    if(bBorder)
    {
        i |= DT_BORDER;
    }
    if(bBKGND)
    {
        i |= DT_BKGND;
    }
//    text=RichTextBox_InsertChar(text,add);
    text=RichTextBox_LineFeed(text,hwnd);
    DrawText(hdc,text,-1,&rc,i);
    EndPaint(hwnd,hdc);

    return true;
}

//���ڱ�������


//��Ϣ��������
//��Ϣ��������
static struct MsgProcTable s_gDrawTextMsgTable[] =
{
    {MSG_CREATE,RichTextBox_Create},         //�����ڴ�����Ϣ
//    {MSG_TIMER,HmiTimer},           //��ʱ����Ϣ
    {MSG_NOTIFY,RichTextBox_Notify},         //�ӿؼ�������֪ͨ��Ϣ
	{MSG_TOUCH_DOWN,RichTextBox_TouchDown},
    {MSG_PAINT,RichTextBox_Paint},           //������Ϣ
//    {MSG_ERASEBKGND,HmiErasebkgnd}, ///���ڱ�������; Param1:��ͼ������;
};
static struct MsgTableLink  s_gDrawTextDemoMsgLink;


void    GDD_Demo_DrawText(void)
{
    s_gDrawTextDemoMsgLink.MsgNum = sizeof(s_gDrawTextMsgTable) / sizeof(struct MsgProcTable);
    s_gDrawTextDemoMsgLink.myTable = (struct MsgProcTable *)&s_gDrawTextMsgTable;
    GDD_CreateGuiApp("DrawText", &s_gDrawTextDemoMsgLink, 0x1000, CN_WINBUF_PARENT);
    GDD_WaitGuiAppExit("DrawText");
}
