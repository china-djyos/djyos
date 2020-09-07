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
//1. ����: 2016-10-25
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include    "gdd.h"
#include    <gdd_widget.h>
#include     <gkernel.h>
#include    "qrencode.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern QRcode *QRcode_encodeString(const char *string, int version,
                                    QRecLevel level, QRencodeMode hint,
                                    int casesensitive);
/*============================================================================*/
/*---------------------------------------------------------------------------
���ܣ�  ���ڴ�������
    ����   :tagWindowMsg *pMsg
    ��� :false��true
---------------------------------------------------------------------------*/
static bool_t HmiCreate_Qrcode(struct WindowMsg *pMsg)
{
    return true;
}
/*---------------------------------------------------------------------------
���ܣ� ���Ӷ�ά�뺯�����еõ�������ת��Ϊָ��洢�������е�����
    ����   :QRcode�ṹ��ָ��
    ��� :�洢�������е�����ָ��
---------------------------------------------------------------------------*/
static uint8_t *Data_conversion(QRcode*qrcode)
{
    u64 i,j,k,n,m;
    uint8_t*Data;
    uint8_t*QrcodeData;
    uint8_t *addr;
    QrcodeData=qrcode->data;
    n=(qrcode->width+7)/8;   //ÿ��ռ���ֽ���
    Data=(uint8_t*)malloc(n*qrcode->width);
    addr=Data;
    m=(qrcode->width)%8;
    for(k=0;k<qrcode->width;k++)
    {
        for(i=1;i<n;i++)//�����ֽ�����
        {
            for(j=0;j<8;j++)
            {
                if((*QrcodeData)&0x01)
                    *Data &=~(0x80>>j);
                else
                    *Data|=(0x80>>j);
                QrcodeData++;
            }
            Data++;
        }
        for(j=0;j<m;j++)//�����ֽ���������
        {
            if((*QrcodeData)&0x01)
                *Data &=~(0x80>>j);
            else
                *Data|=(0x80>>j);
            QrcodeData++;
        }
        Data++;
    }

    return addr;
}


static uint8_t __transfer(uint8_t c)
{
    uint8_t tmp=0;
    if(c&0x1)
        tmp+=0x3;
    if(c&0x2)
        tmp+=0xc;
    if(c&0x4)
        tmp+=0x30;
    if(c&0x8)
        tmp+=0xc0;
    return tmp;
}

static uint8_t *__BitmapEnlarge(uint8_t *srcbm,uint8_t wid,uint8_t bytes)
{
    uint8_t *dstbm;
    uint8_t i,j,c,tmp1,tmp2;
    uint16_t len;
    len=bytes*wid;
    dstbm=(uint8_t *)malloc(4*len);
    if(dstbm==NULL)
        return NULL;
    for(i=0;i<wid;i++)
    {
        for(j=0;j<bytes;j++)
        {
            c=*(srcbm+i*bytes+j);
            //ȡ����λ
            tmp1=c;
            tmp1=tmp1&0xf0;
            tmp1=tmp1>>4;
            tmp2=__transfer(tmp1);
            *(dstbm+2*i*bytes*2+j*2)=tmp2;
            *(dstbm+(2*i+1)*bytes*2+j*2)=tmp2;
            //ȡ����λ
            tmp1=c;
            tmp1=tmp1&0xf;
            tmp2=__transfer(tmp1);
            *(dstbm+2*i*bytes*2+j*2+1)=tmp2;
            *(dstbm+(2*i+1)*bytes*2+j*2+1)=tmp2;
        }

    }
    return dstbm;
}
/*---------------------------------------------------------------------------
���ܣ�������������õ���ά����Ϣ
    ����   :string: �����������
        version: �汾��<=40     ���=(�汾��*4+17)
        level: �ݴ�ȼ�L��Low����7%������ɱ�����
               M��Medium����15%������ɱ�����
               Q��Quartile����25%������ɱ�����
               H��High����30%������ɱ�����
        hint: ���뷽ʽ
        casesensitive: �Ƿ����ִ�Сд
    ��� :�洢�������е�����ָ��
---------------------------------------------------------------------------*/
//������Ϣ������
static bool_t HmiPaint_Qrcode(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC  hdc;
    RECT rc0;
    s32 height,width,size,bytes;
    u8 vernum;
    char *str;
    QRcode*qrcode; //�����
    struct RectBitmap   bitmap;
    struct RopGroup RopCode = (struct RopGroup){ 0, 0, 0, CN_R2_COPYPEN, 0, 0 };
    u8 *Data,*bm;
    if(pMsg==NULL)
        return false;
    hwnd=pMsg->hwnd;
    if(hwnd==NULL)
        return false;
    hdc =GDD_BeginPaint(hwnd);
    if(hdc == NULL)
        return false;
    GDD_GetClientRect(hwnd,&rc0);
    GDD_SetDrawColor(hdc,CN_COLOR_BLACK);
    width=GDD_RectW(&rc0);
    height=GDD_RectH(&rc0);
    if(width<=height)
    {
        size=width;
    }
    else
    {
        size=height;
    }
    size=size/2;
    //���ݳߴ�����ά���С
    vernum=(size-17)/4;
    str=GDD_GetWindowText(hwnd);
/* 89x89λ��Ķ�ά�����Ϊ
 * �汾��:18
 * �ݴ�ȼ�  (ECC)  ����  (����)     ��ĸ                                ����          �����ƴ���
 * L            1,725       1,046       442    586
 * M            1,346       816         345    450
 * Q            948         574         243    322
 * H            746         452         191    250
 * */
   qrcode= QRcode_encodeString(str,vernum, QR_ECLEVEL_H, QR_MODE_8,0);
   if(qrcode==NULL)//�洢�ռ䲻����������������涨��Χ
   {
       QRcode_free(qrcode);
       GDD_EndPaint(hwnd,hdc);
       return false;
   }
    Data=Data_conversion(qrcode);//��ȡ��ʾ���ݵ�Data
    bytes=(qrcode->width+7)/8;   //ÿ��ռ���ֽ���
    bm=__BitmapEnlarge(Data,size,bytes);   //ÿ��ռ���ֽ���);

    bitmap.bm_bits = bm;
    bitmap.linebytes = 2*((qrcode->width+7)/8);
    bitmap.PixelFormat = CN_SYS_PF_GRAY1;
    bitmap.ExColor = CN_COLOR_WHITE;
    bitmap.height=(s32)(qrcode->width)*2;
    bitmap.width=(s32)(qrcode->width)*2;
    GDD_DrawBitmap(hdc,rc0.left,rc0.top,&bitmap,CN_SYS_PF_GRAY1,RopCode);
    GDD_EndPaint(hwnd,hdc);
    QRcode_free(qrcode);
    free(Data);
    free(bm);

    return true;
}

//��Ϣ��������
static struct MsgProcTable s_gQrcodeMsgProcTable[] =
{
    {MSG_CREATE,HmiCreate_Qrcode},         //�����ڴ�����Ϣ
    {MSG_PAINT,HmiPaint_Qrcode}           //������Ϣ
};

static struct MsgTableLink  s_gQrcodeMsgLink;


HWND CreateQrcode(  const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,ptu32_t pdata,
                    struct MsgTableLink *UserMsgTableLink)
{
    HWND pGddWin;
    s_gQrcodeMsgLink.MsgNum = sizeof(s_gQrcodeMsgProcTable) / sizeof(struct MsgProcTable);
    s_gQrcodeMsgLink.myTable = (struct MsgProcTable *)&s_gQrcodeMsgProcTable;
    pGddWin=GDD_CreateWindow(Text,WS_CHILD |Style,x,y,w,h,hParent,WinId,
                              CN_WINBUF_PARENT,pdata,&s_gQrcodeMsgLink);
    if(UserMsgTableLink != NULL)
          GDD_AddProcFuncTable(pGddWin,UserMsgTableLink);
    return pGddWin;
}
