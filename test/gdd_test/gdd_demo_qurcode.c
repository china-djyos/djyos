/*============================================================================*/

#include <gdd.h>
#include <gkernel.h>
#include <qrencode.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*---------------------------------------------------------------------------
功能：  窗口创建函数
    输入   :tagWindowMsg *pMsg
    输出 :false或true
---------------------------------------------------------------------------*/
static bool_t HmiCreate_Qrcode(struct WindowMsg *pMsg)
{


    return true;
}
/*---------------------------------------------------------------------------
功能： 将从二维码函数库中得到的数据转换为指向存储像素阵列的数组
    输入   :QRcode结构体指针
    输出 :存储像素阵列的数组指针
---------------------------------------------------------------------------*/
static u8 *Data_conversion(QRcode*qrcode)
{
    u64 i,j,k,n,m;
    u8*Data;
    u8*QrcodeData;
    u8 *addr;
    QrcodeData=qrcode->data;
    n=(qrcode->width+7)/8;   //每行占得字节数
    Data=(u8*)malloc(n*qrcode->width);
    addr=Data;
    m=(qrcode->width)%8;
    for(k=0;k<qrcode->width;k++)
    {
        for(i=1;i<n;i++)//复制字节整数
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
        for(j=0;j<m;j++)//复制字节余数部分
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
功能：根据输入参数得到二维码信息
    输入   :string: 被编码的数据
        version: 版本号<=40     宽度=(版本号*4+17)
        level: 容错等级L（Low）：7%的字码可被修正
               M（Medium）：15%的字码可被修正
               Q（Quartile）：25%的字码可被修正
               H（High）：30%的字码可被修正
        hint: 编码方式
        casesensitive: 是否区分大小写
    输出 :存储像素阵列的数组指针
---------------------------------------------------------------------------*/
extern QRcode *QRcode_encodeString(const char *string, int version,
                                    QRecLevel level, QRencodeMode hint,
                                    int casesensitive);

//绘制消息处函数
static bool_t HmiPaint_Qrcode(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC  hdc;
    RECT rc0;

    QRcode*qrcode; //最后结果
    struct RectBitmap   bitmap;
    struct RopGroup RopCode = (struct RopGroup){ 0, 0, 0, CN_R2_COPYPEN, 0, 0 };
    u8 *Data;

    hwnd =pMsg->hwnd;
    hdc =GDD_BeginPaint(hwnd);
    GDD_GetClientRect(hwnd,&rc0);

    GDD_SetDrawColor(hdc,CN_COLOR_BLACK);
/* 89x89位宽的二维码参数为
 * 版本号:18
 * 容错等级  (ECC)  数字  (容量)     字母                                汉字          二进制代码
 * L            1,725       1,046       442    586
 * M            1,346       816         345    450
 * Q            948         574         243    322
 * H            746         452         191    250
 * */
    qrcode= QRcode_encodeString("AbcdEfg;",10, QR_ECLEVEL_H, QR_MODE_8,0);
   if(qrcode==NULL)//存储空间不足或数据容量超出规定范围
   {
       QRcode_free(qrcode);
       return false;
   }
    Data=Data_conversion(qrcode);//提取显示数据到Data
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

//消息处理函数表
static struct MsgProcTable s_gQrcodeMsgTable[] =
{
    {MSG_CREATE,HmiCreate_Qrcode},         //主窗口创建消息
    {MSG_PAINT,HmiPaint_Qrcode},           //绘制消息
};

static struct MsgTableLink  s_gQrcodeDemoMsgLink;

void    GDD_Demo_Qrcode(void)
{
    s_gQrcodeDemoMsgLink.MsgNum = sizeof(s_gQrcodeMsgTable) / sizeof(struct MsgProcTable);
    s_gQrcodeDemoMsgLink.myTable = (struct MsgProcTable *)&s_gQrcodeMsgTable;
    GDD_CreateGuiApp("二维码", &s_gQrcodeDemoMsgLink, 0x800, CN_WINBUF_PARENT,WS_BORDER|WS_DLGFRAME|WS_CAPTION|WS_SYSMENU);
    GDD_WaitGuiAppExit("二维码");

}
