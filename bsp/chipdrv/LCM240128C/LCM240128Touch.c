//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================

// �ļ���     ��crtouch.c
// ģ������: ����оƬCRTOUCH����
// ģ��汾: V1.10
// ������Ա: zhb
// ����ʱ��: 03/27.2017
// =============================================================================

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "stdint.h"
#include "stdlib.h"
#include "int_hard.h"
#include "int.h"
#include "cpu_peri.h"
#include "Touch.h"
#include "spibus.h"
#include "gkernel.h"
#include "systime.h"
#include <device/djy_flash.h>
#include <device.h>
//#include "at24c128b.h"
#include "Touch.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_LCM240128Touch(void);
//    ModuleInstall_LCM240128Touch( );
//#if(CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT == true)
//    extern bool_t GDD_AddInputDev(const char *InputDevName);
//    GDD_AddInputDev(CFG_LCM240128_TOUCH_NAME);
//#endif
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"chip driver lcm240128c"//iic�ӿڵĴ���������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:later              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"io analog iic bus","graphical kernel"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"graphical decorate development"          //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"non"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_LCM240128_TOUCH_NAME        //****�������Ƿ��Ѿ����ú�
#warning    LCM240128C�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,1,100,
//%$#@enum,true,false,
//%$#@string,1,128,
#define CFG_LCM240128_TOUCH_NAME   "LCM240128C"  //"�����豸����",
#define CFG_DISPLAY_NAME           "display"       //"�����������������",
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

#define SPI_BUS_NAME           "SPI5"
#define CN_TOUCH_TIMEOUT        10000*mS
struct AdjustValue
{
    s16 XAdjustLeft;
    s16 YAdjustTop;
    s16 XAdjustRight;
    s16 YAdjustBottom;
};

static s32 xp,yp,z;

extern const Pin PenStatus[1];
extern const Pin SpiCs[1];


#define CN_TOUCH_FLAG_SIZE         (8)//��־λ��С
#define CN_TOUCH_ADJUST_SIZE       (8)//��Ϣλ��С
#define CN_EEPROM_TOUCH_ADJUST_ADDR       (0x4000-CN_TOUCH_FLAG_SIZE -CN_TOUCH_ADJUST_SIZE)
static const u8  buf_flag[CN_TOUCH_FLAG_SIZE]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
extern u32 AT24_ReadBytes(u32 wAddr, u8 *pbyBuf, u32 wBytesNum);
extern u32 AT24_WriteBytes(u32 wAddr, u8 *pbyBuf, u32 wBytesNum);

static  s16 gs_XadjustLeft=0,gs_XadjustRight=0;
static  s16 gs_YadjustTop=0,gs_YadjustBottom=0;

//����SPIBUS�ܹ��µ�SPI�豸�ṹ
static struct SPI_Device *s_ptTouch_Dev;

bool_t Touch_EraseAdjustVal(void)
{
    bool_t ret;
    u8 i, wbuf[CN_TOUCH_ADJUST_SIZE+CN_TOUCH_FLAG_SIZE];
    for(i=0;i<(CN_TOUCH_ADJUST_SIZE+CN_TOUCH_FLAG_SIZE);i++)
    {
        wbuf[i]=0xFF;
    }

    ret=AT24_WriteBytes(CN_EEPROM_TOUCH_ADJUST_ADDR,wbuf,\
            (CN_TOUCH_ADJUST_SIZE+CN_TOUCH_FLAG_SIZE));

    if(!ret)
    {
         printf("Erase touch  adjust val failed.\r\n");
         return false;
    }
    printf("Erase touch adjust val success.\r\n");
    return true;
}


void Touch_GetAdjustVal(void)
{
    s16 s_s16gXAdjustLeft,s_s16gXAdjustRight;
    s16 s_s16gYAdjustTop,s_s16gYAdjustBottom;
    u8 rbuf[CN_TOUCH_ADJUST_SIZE+CN_TOUCH_FLAG_SIZE];
    bool_t ret;
    u16 tmp=0;
    u8 i;

    ret=AT24_ReadBytes(CN_EEPROM_TOUCH_ADJUST_ADDR,rbuf\
                        ,(CN_TOUCH_ADJUST_SIZE+CN_TOUCH_FLAG_SIZE));
    if(!ret)
    {
        printf("Read touch adjust value failed.\r\n");
        return;
    }
    for(i=0;i<8;i++)
    {
        if(rbuf[i]!=buf_flag[i])
        {
            printf("Touch write adjust value error.\r\n");
            return;
        }

    }

    s_s16gXAdjustLeft=rbuf[8];
    tmp=rbuf[9];
    tmp=tmp<<8;
    s_s16gXAdjustLeft|=tmp;
    tmp=0;

    s_s16gYAdjustTop=rbuf[10];
    tmp=rbuf[11];
    tmp=tmp<<8;
    s_s16gYAdjustTop|=tmp;
    tmp=0;

    s_s16gXAdjustRight=rbuf[12];
    tmp=rbuf[13];
    tmp=tmp<<8;
    s_s16gXAdjustRight|=tmp;
    tmp=0;

    s_s16gYAdjustBottom=rbuf[14];
    tmp=rbuf[15];
    tmp=tmp<<8;
    s_s16gYAdjustBottom|=tmp;
    printf("EEPROM:  ");
    printf("x1=%d,y1=%d\n\r",s_s16gXAdjustLeft,s_s16gYAdjustTop);
    printf("x2=%d,y2=%d\n\r",s_s16gXAdjustRight,s_s16gYAdjustBottom);
    printf("RAM:  ");
    printf("x1=%d,y1=%d\n\r",gs_XadjustLeft,gs_YadjustTop);
    printf("x2=%d,y2=%d\n\r",gs_XadjustRight,gs_YadjustBottom);
}

static bool_t gs_bTouchDbgFlg=false;
void Touch_GetTouchPoint(void)
{
    gs_bTouchDbgFlg=true;
}

// =============================================================================
// ���ܣ�����У׼�õ���У׼ֵд��Ƭ��Flash�����8���ֽ��С�
// ������pAdjustValue,У׼ֵ�ṹ��ָ��
// ���أ�true,д�ɹ�;false,дʧ��
// =============================================================================
static bool_t Touch_WriteAdjustValue(struct AdjustValue* pAdjustValue)
{
    u8 wbuf[CN_TOUCH_ADJUST_SIZE+CN_TOUCH_FLAG_SIZE];
    s32 ret;
    u8 i;
    if(pAdjustValue==NULL)
        return false;
    for(i=0;i<CN_TOUCH_FLAG_SIZE;i++)
        wbuf[i]=buf_flag[i];
    wbuf[0+CN_TOUCH_FLAG_SIZE]=pAdjustValue->XAdjustLeft;
    wbuf[1+CN_TOUCH_FLAG_SIZE]=pAdjustValue->XAdjustLeft>>8;

    wbuf[2+CN_TOUCH_FLAG_SIZE]=pAdjustValue->YAdjustTop;
    wbuf[3+CN_TOUCH_FLAG_SIZE]=pAdjustValue->YAdjustTop>>8;

    wbuf[4+CN_TOUCH_FLAG_SIZE]=pAdjustValue->XAdjustRight;
    wbuf[5+CN_TOUCH_FLAG_SIZE]=pAdjustValue->XAdjustRight>>8;

    wbuf[6+CN_TOUCH_FLAG_SIZE]=pAdjustValue->YAdjustBottom;
    wbuf[7+CN_TOUCH_FLAG_SIZE]=pAdjustValue->YAdjustBottom>>8;

    ret=AT24_WriteBytes(CN_EEPROM_TOUCH_ADJUST_ADDR,wbuf,\
                CN_TOUCH_ADJUST_SIZE+CN_TOUCH_FLAG_SIZE);
    if(!ret)
    {
        printf("Write touch adjust value failed.\r\n");
        return false;
    }

    return true;
}


// =============================================================================
// ���ܣ���ȡ������AD����ֵ���ֱ�ΪXֵ��Yֵ��δ��У��������
// ������X,Y������������洢��ȡ�������ݵ�ַ
// ���أ�true,��ȡ�ɹ�;false,��ȡʧ��
// =============================================================================
static bool_t Touch_ReadXY(s16 *X, s16 *Y)
{
    struct SPI_DataFrame data;
    u8 xybuf[] = {  0xd0,00,00,0x90,00,00,
                    0xd0,00,00,0x90,00,00,
                    0xd0,00,00,0x90,00,00,
                    0xd0,00,00,0x90,00,00};
    u16 maxx=0,minx=65535,maxy=0,miny=65535;
    s16 x0[4],y0[4];
    u32 loop;
    s32 tempx=0,tempy=0;

#if 1
    data.SendBuf = xybuf;
    data.SendLen = 24;
    data.RecvBuf = xybuf;
    data.RecvLen = 23;
    data.RecvOff = 1;
    //��4���������ֵ
    PIO_Clear(SpiCs);
    SPI_Transfer(s_ptTouch_Dev, &data, true, CN_TIMEOUT_FOREVER);
    DJY_EventDelay(1000);
    PIO_Set(SpiCs);

#else

    data.SendBuf = xybuf;
    data.SendLen = 1;
    data.RecvBuf = xybuf;
    data.RecvLen = 2;
    data.RecvOff = 1;
    result = SPI_Transfer(s_ptTouch_Dev, &data, true, CN_TIMEOUT_FOREVER);

    data.SendBuf = xybuf+2;
    data.SendLen = 1;
    data.RecvBuf = xybuf+2;
    data.RecvLen = 2;
    data.RecvOff = 1;
    result = SPI_Transfer(s_ptTouch_Dev, &data, true, CN_TIMEOUT_FOREVER);
#endif
    //������һ�������㣨��׼����ȥ������3�������㣬����ȡ�����Сֵ
    for(loop = 1;loop<4;loop++)
    {
        x0[loop] = (xybuf[loop *6] << 8) +xybuf[loop*6+1];
        x0[loop] >>=3;
        y0[loop] = (xybuf[loop*6+3] << 8) +xybuf[loop*6+4];
        y0[loop] >>=3;
        if(x0[loop] > maxx)
            maxx = x0[loop];
        if(x0[loop] < minx)
            minx = x0[loop];
        if(y0[loop] > maxy)
            maxy = y0[loop];
        if(y0[loop] < miny)
            miny = y0[loop];
    }

    //�����Сֵ֮�����20�Ļ���˵������ֵ���ȶ��������ڶ�����
    if( (maxx - minx > 20) || (maxy - miny > 20) )
        return false;
    else
    {
        for(loop = 1;loop<4;loop++)
        {
            tempx +=x0[loop];
            tempy +=y0[loop];
        }
        tempx -= minx;      //�������ֵ����Сֵ
        tempx -= maxx;
        tempy -= miny;
        tempy -= maxy;
        *X = tempx / 1;
        *Y = tempy / 1;
        return true;
    }
}

// =============================================================================
// ���ܣ���ȡ��ʱ�Ƿ��������д���
// ��������
// ���أ�1,����;0,δ����
// =============================================================================
static bool_t Touch_Touched(void)
{
    u8 TouchS;
    TouchS = PIO_Get(PenStatus);
    if(TouchS ==0)
        return true;
    else
        return false;
}


//----��ȡ����������-----------------------------------------------------------
//����: ��ȡstmpe811�ɼ����Ĵ��������꣬����ж�㣬��ƽ��֮
//����: touch_data���ɼ���������
//����: 1=�����ʰ��£�0=����������
//-----------------------------------------------------------------------------
static ufast_t ReadTouch(struct SingleTouchMsg *touch_data)
{
    atom_low_t atom;
    touch_data->display = NULL;
    atom = Int_LowAtomStart( );
    touch_data->x =xp;
    touch_data->y =yp;
    touch_data->z = z;
    Int_LowAtomEnd(atom);
    return z;

}

//----��ȡ����������-----------------------------------------------------------
//����: ��ȡstmpe811�ɼ����Ĵ��������꣬����ж�㣬��ƽ��֮
//����: touch_data���ɼ���������
//����: 1=�����ʰ��£�0=����������
//-----------------------------------------------------------------------------
static ptu32_t ScanTouch(void)
{
    bool_t trans;
    atom_low_t atom;
    s16 x,y;
    s32 count =  0;

    while(1)
    {
        trans = Touch_Touched( );
        if(trans)
        {
            if(count++ > 2)
            {
                DJY_EventDelay(1000);

                trans = Touch_ReadXY(&x,&y);

                trans &= Touch_Touched( );
                if(trans)
                {
                    if((gs_XadjustRight!=gs_XadjustLeft)&&(gs_YadjustBottom!=gs_YadjustTop))
                    {
                        x = 200 * (s32)(x - gs_XadjustLeft)/(gs_XadjustRight - gs_XadjustLeft)+20;
                        y = 88 * (s32)(y - gs_YadjustTop)/(gs_YadjustBottom - gs_YadjustTop)+20;

                        if( ( ( (xp -x)*(xp-x) + (yp-y)*(yp-y) ) >= 36) && (count >5 ) )
                        {
                            //��������������� �β���֮���ƶ�����ﵽ6�����أ�˵������ʹ����е����,������
                        }
                        else
                        {
                            atom = Int_LowAtomStart( );
                            xp = x;
                            yp = y;
                            z = 1;
                            Int_LowAtomEnd(atom);
                            if(gs_bTouchDbgFlg==true)
                            {
                                printf("x=%d,y=%d\n\r",xp,yp);
                                gs_bTouchDbgFlg=false;
                            }
                        }
                        count = 5;
                    }
                }
                else
                    count = 0;
            }
        }
        else
        {
            if(count-- < -2)
            {
                count = -3;
                z = 0;
            }
        }
        DJY_EventDelay(10*mS);
    }
    return 0;
}

static  void draw_cursor(struct GkWinObj *desktop,int x,int y)
{
    GK_ApiLineto(desktop,x,y-9,x,y+9,CN_COLOR_WHITE,CN_R2_COPYPEN,0); //��
    GK_ApiLineto(desktop,x-9,y,x+9,y,CN_COLOR_WHITE,CN_R2_COPYPEN,0);  //��
    GK_ApiSyncShow(1000*mS);
}

static  void clr_cursor(struct GkWinObj *desktop,s32 x,s32 y)
{
    GK_ApiLineto(desktop,x,y-9,x,y+9,CN_COLOR_BLACK,CN_R2_COPYPEN,0); //��
    GK_ApiLineto(desktop,x-9,y,x+9,y,CN_COLOR_BLACK,CN_R2_COPYPEN,0);  //��
    GK_ApiSyncShow(1000*mS);

}



void touch_ratio_adjust(struct GkWinObj *desktop)
{
    u32 loop;
    s32 tempx,tempy;
    u16 maxx=0,minx=65535,maxy=0,miny=65535;
    s16 x[7],y[7];
    struct AdjustValue* pAdjustValue;
//    s64 temp=0;
    bool_t result=false;
    s16 s_s16gXAdjustLeft,s_s16gXAdjustRight;
    s16 s_s16gYAdjustTop,s_s16gYAdjustBottom;
    u8 rbuf[CN_TOUCH_ADJUST_SIZE+CN_TOUCH_FLAG_SIZE];
    u16 tmp=0;
    u8 i,j,cnt=0;
    pAdjustValue=(struct AdjustValue*)malloc(sizeof(struct AdjustValue));
    if(pAdjustValue==NULL)
        return;
    tempx = 0;
    tempy = 0;
    GK_ApiFillWin(desktop,CN_COLOR_BLACK,0);

    result=AT24_ReadBytes(CN_EEPROM_TOUCH_ADJUST_ADDR,\
                rbuf,CN_TOUCH_FLAG_SIZE+CN_TOUCH_ADJUST_SIZE);
    if(!result)
    {
        printf("Read touch adjust value failed.\r\n");
        return;
    }

    gs_XadjustLeft=rbuf[0+CN_TOUCH_FLAG_SIZE];
    tmp=rbuf[1+CN_TOUCH_FLAG_SIZE];
    tmp=tmp<<8;
    gs_XadjustLeft|=tmp;
    tmp=0;

    gs_YadjustTop=rbuf[2+CN_TOUCH_FLAG_SIZE];
    tmp=rbuf[3+CN_TOUCH_FLAG_SIZE];
    tmp=tmp<<8;
    gs_YadjustTop|=tmp;
    tmp=0;

    gs_XadjustRight=rbuf[4+CN_TOUCH_FLAG_SIZE];
    tmp=rbuf[5+CN_TOUCH_FLAG_SIZE];
    tmp=tmp<<8;
    gs_XadjustRight|=tmp;
    tmp=0;

    gs_YadjustBottom=rbuf[6+CN_TOUCH_FLAG_SIZE];
    tmp=rbuf[7+CN_TOUCH_FLAG_SIZE];
    tmp=tmp<<8;
    gs_YadjustBottom|=tmp;

    for(i=0;i<CN_TOUCH_FLAG_SIZE;i++)
    {
        if(rbuf[i]!=buf_flag[i])
            break;
    }

    //���ж�Flash���Ƿ��Ѵ洢��Ч��У׼ֵ   ����ҪУ��
    if(i!=CN_TOUCH_FLAG_SIZE)
    {
        printf("\r\n");
        printf("LCM240128C TOUCH Adjust.");
        GK_ApiDrawText(desktop,NULL,NULL,40,20,
                        "���������� ",11,CN_COLOR_WHITE,CN_R2_COPYPEN,0);
        GK_ApiDrawText(desktop,NULL,NULL,40,40,
                        "��׼ȷ���ʮ�ֽ����",20,CN_COLOR_WHITE,CN_R2_COPYPEN,0);
        draw_cursor(desktop, 20, 20);
//        while(Touch_Touched() == false);
        for(j=0;j<20;j++)
        {
            if(Touch_Touched() == false)
            {
                printf(".");
                DJY_EventDelay(1000*mS);
            }
            else
            {
                break;
            }
        }
        if(j==20)
        {
            printf("\r\nLCM240128C TOUCH TIMEOUT,Adjust failed!!!\r\n");
            return;
        }

        DJY_EventDelay(100*mS);

        for(loop = 0;loop < 50; )
        {
            if(Touch_Touched())
            {
                loop++;
                DJY_DelayUs(30000);
            }
        }
        for(loop = 0;loop < 7; )
        {
            if(Touch_Touched())
            {
                if(Touch_ReadXY(&x[loop], &y[loop]))
                {
                    if(Touch_Touched())
                    {
                        tempx +=x[loop];
                        tempy +=y[loop];
                        if(x[loop] > maxx)
                            maxx = x[loop];
                        if(x[loop] < minx)
                            minx = x[loop];
                        if(y[loop] > maxy)
                            maxy = y[loop];
                        if(y[loop] < miny)
                            miny = y[loop];
                        loop++;
                    }
                }
                DJY_DelayUs(30000);
            }
        }
        tempx -= maxx + minx;
        tempy -= maxy + miny;
        s_s16gXAdjustLeft = tempx / 5;
        s_s16gYAdjustTop = tempy / 5;

        //    printk("x=%d,y=%d\n\r",s_s16gXAdjustLeft,s_s16gYAdjustTop);
        //    s_s16gXAdjustLeft = 0;
        //    s_s16gYAdjustTop = 0;
        //    maxx=0,minx=65535,maxy=0,miny=65535,tempx=0,tempy=0;
        //    DJY_DelayUs(300*mS);
        //    }
        clr_cursor(desktop, 20, 20);
        draw_cursor(desktop, 220, 108);

//        while(Touch_Touched() == true);
//        DJY_EventDelay(300*mS);

//        while(Touch_Touched() == false);
        for(j=0;j<20;j++)
        {
            if(Touch_Touched() == false)
            {
                printf(".");
                DJY_EventDelay(1000*mS);
            }
            else
            {
                break;
            }
        }
        if(j==20)
        {
            printf("\r\nLCM240128C TOUCH TIMEOUT,Adjust failed.\r\n!!!\r\n");
            return;
        }
        DJY_EventDelay(100*mS);
        maxx=0;
        minx=65535;
        maxy=0;
        miny=65535;
        tempx = 0;
        tempy = 0;
        for(loop = 0;loop < 50; )
        {
            if(Touch_Touched())
            {
                loop++;
                DJY_DelayUs(30000);
            }
        }
        for(loop = 0;loop < 7; )
        {
            if(Touch_Touched())
            {
                if(Touch_ReadXY(&x[loop], &y[loop]))
                {
                    if(Touch_Touched())
                    {
                        tempx +=x[loop];
                        tempy +=y[loop];
                        if(x[loop] > maxx)
                            maxx = x[loop];
                        if(x[loop] < minx)
                            minx = x[loop];
                        if(y[loop] > maxy)
                            maxy = y[loop];
                        if(y[loop] < miny)
                            miny = y[loop];
                        loop++;
                    }
                }
                DJY_DelayUs(30000);
            }
        }
        tempx -= maxx +minx;
        tempy -= maxy +miny;
        s_s16gXAdjustRight = tempx / 5;
        s_s16gYAdjustBottom = tempy / 5;
        clr_cursor(desktop, 220, 108);

        pAdjustValue->XAdjustLeft=s_s16gXAdjustLeft;
        pAdjustValue->XAdjustRight=s_s16gXAdjustRight;
        pAdjustValue->YAdjustBottom=s_s16gYAdjustBottom;
        pAdjustValue->YAdjustTop=s_s16gYAdjustTop;


        //���ݴ���
        if((s_s16gXAdjustLeft==s_s16gXAdjustRight)||(s_s16gYAdjustBottom==s_s16gYAdjustTop))
        {
            printf("TOUCH Adjust failed.TOUCH invalid.\r\n");
            return;
        }

        result=Touch_WriteAdjustValue(pAdjustValue);

        if(result)
        {
            GK_ApiDrawText(desktop,NULL,NULL,50,600,"У׼�ɹ�",
                                        8,CN_COLOR_WHITE,CN_R2_COPYPEN,0);
            gs_XadjustLeft=s_s16gXAdjustLeft;
            gs_XadjustRight=s_s16gXAdjustRight;
            gs_YadjustBottom=s_s16gYAdjustBottom;
            gs_YadjustTop=s_s16gYAdjustTop;
            DJY_EventDelay(200*mS);
        }

        else
        {
           return;
        }
    }

}
//----��ʼ��������ģ��---------------------------------------------------------
//����:
//����: display_dev_name:����������Ӧ����ʾ����(��Դ��)
//      touch_dev_name:�������豸��.
//����: ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_LCM240128Touch(void)
{
    struct GkWinObj *desktop;
    static struct SingleTouchPrivate touch_dev;
    u16 evtt_id;
    s16 tmp_x=0,tmp_y=0;
    PIO_Clear(SpiCs);
    DJY_EventDelay(100*mS);
    PIO_Set(SpiCs);
    DJY_EventDelay(100*mS);

    //�޸�byzhb20170325   ԭ����1M��Ϊ500k���������ڱ�Եʱ��׼��
    s_ptTouch_Dev = SPI_DevAdd(SPI_BUS_NAME,CFG_LCM240128_TOUCH_NAME,0,8,SPI_MODE_0,
                                SPI_SHIFT_MSB,500000,false)
    if(NULL != s_ptTouch_Dev)
    {
        SPI_BusCtrl(s_ptTouch_Dev,CN_SPI_SET_POLL,0,0);
    }
    else
    {
        printf("��װ������SPI��������\n\r");
        return false;
    }
    desktop = GK_GetDesktop(CFG_DISPLAY_NAME);

    Touch_ReadXY(&tmp_x, &tmp_y);
    touch_ratio_adjust(desktop);

    touch_dev.read_touch = ReadTouch;
    touch_dev.touch_loc.display = GK_GetDisplay(CFG_DISPLAY_NAME);
    Touch_InstallDevice(CFG_LCM240128_TOUCH_NAME,&touch_dev);

    evtt_id = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
                                ScanTouch,NULL,1024,
                                "touch scaning");
    //�¼���������������Ϊ0?�����shell����?�������ɼ�shell�����в���
    DJY_EventPop(evtt_id,NULL,0,0,0,0);

    return true;
}


