/*============================================================================*/

#include <gdd.h>
#include <gkernel.h>
#include <qrencode.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
static u8 *Data_conversion(QRcode*qrcode)
{
    u64 i,j,k,n,m;
    u8*Data;
    u8*QrcodeData;
    u8 *addr;
    QrcodeData=qrcode->data;
    n=(qrcode->width+7)/8;   //ÿ��ռ���ֽ���
    Data=(u8*)malloc(n*qrcode->width);
    addr=Data;
    m=(qrcode->width)%8;
    for(k=0;k<qrcode->width;k++)
    {
        for(i=1;i<n;i++)//�����ֽ�����
        {
            for(j=0;j<8;j++)
            {
                if((*QrcodeData)&0x01)
                    *Data|=(0x80>>j);
                else
                    *Data &=~(0x80>>j);
                QrcodeData++;
            }
            Data++;
        }
        for(j=0;j<m;j++)//�����ֽ���������
        {
            if((*QrcodeData)&0x01)
                *Data|=(0x80>>j);
            else
                *Data &=~(0x80>>j);
            QrcodeData++;
        }
        Data++;
    }
    return addr;
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
extern QRcode *QRcode_encodeString(const char *string, int version,
                                    QRecLevel level, QRencodeMode hint,
                                    int casesensitive);

//������Ϣ������
static bool_t HmiPaint_Qrcode(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC  hdc;
    RECT rc0;

    QRcode*qrcode; //�����
    struct RectBitmap   bitmap;
    struct RopGroup RopCode = (struct RopGroup){ 0, 0, 0, CN_R2_COPYPEN, 0, 0 };
    u8 *Data;

    hwnd =pMsg->hwnd;
    hdc =GDD_BeginPaint(hwnd);
    GDD_GetClientRect(hwnd,&rc0);

    GDD_SetDrawColor(hdc,CN_COLOR_BLACK);
/* 89x89λ��Ķ�ά�����Ϊ
 * �汾��:18
 * �ݴ�ȼ�  (ECC)  ����  (����)     ��ĸ                                ����          �����ƴ���
 * L            1,725       1,046       442    586
 * M            1,346       816         345    450
 * Q            948         574         243    322
 * H            746         452         191    250
 * */
    qrcode= QRcode_encodeString("AbcdEfg;",10, QR_ECLEVEL_H, QR_MODE_8,0);
   if(qrcode==NULL)//�洢�ռ䲻����������������涨��Χ
   {
       QRcode_free(qrcode);
       return false;
   }
    Data=Data_conversion(qrcode);//��ȡ��ʾ���ݵ�Data
    bitmap.bm_bits = Data;
    bitmap.linebytes = (qrcode->width+7)/8;
    bitmap.PixelFormat = CN_SYS_PF_GRAY1;
    bitmap.ExColor = CN_COLOR_WHITE;
    bitmap.height=(s32)(qrcode->width);
    bitmap.width=(s32)(qrcode->width);
    GDD_DrawBitmap(hdc,rc0.left+75,rc0.top+20,&bitmap,CN_SYS_PF_GRAY1,RopCode);

    GDD_EndPaint(hwnd,hdc);
    QRcode_free(qrcode);
    free(Data);
    return true;
}

//��Ϣ��������
static struct MsgProcTable s_gQrcodeMsgTable[] =
{
    {MSG_CREATE,HmiCreate_Qrcode},         //�����ڴ�����Ϣ
    {MSG_PAINT,HmiPaint_Qrcode},           //������Ϣ
};

static struct MsgTableLink  s_gQrcodeDemoMsgLink;

void    GDD_Demo_Qrcode(void)
{
    s_gQrcodeDemoMsgLink.MsgNum = sizeof(s_gQrcodeMsgTable) / sizeof(struct MsgProcTable);
    s_gQrcodeDemoMsgLink.myTable = (struct MsgProcTable *)&s_gQrcodeMsgTable;
    GDD_CreateGuiApp("��ά��", &s_gQrcodeDemoMsgLink, 0x800, CN_WINBUF_PARENT,WS_BORDER|WS_DLGFRAME|WS_CAPTION|WS_SYSMENU);
    GDD_WaitGuiAppExit("��ά��");

}
