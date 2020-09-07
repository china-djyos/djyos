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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// =============================================================================

// 文件名     ：crtouch.c
// 模块描述: 触摸芯片CRTOUCH驱动
// 模块版本: V1.10
// 创建人员: zhb
// 创建时间: 03/27.2017
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

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_LCM240128Touch(void);
//    ModuleInstall_LCM240128Touch( );
//#if(CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT == true)
//    extern bool_t GDD_AddInputDev(const char *InputDevName);
//    GDD_AddInputDev(CFG_LCM240128_TOUCH_NAME);
//#endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"chip driver lcm240128c"//iic接口的触摸屏控制
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:later              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"io analog iic bus","graphical kernel"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"graphical decorate development"          //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"non"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#ifndef CFG_LCM240128_TOUCH_NAME        //****检查参数是否已经配置好
#warning    LCM240128C组件参数未配置，使用默认值
//%$#@num,1,100,
//%$#@enum,true,false,
//%$#@string,1,128,
#define CFG_LCM240128_TOUCH_NAME   "LCM240128C"  //"触摸设备名称",
#define CFG_DISPLAY_NAME           "display"       //"触摸所在桌面的名称",
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
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


#define CN_TOUCH_FLAG_SIZE         (8)//标志位大小
#define CN_TOUCH_ADJUST_SIZE       (8)//信息位大小
#define CN_EEPROM_TOUCH_ADJUST_ADDR       (0x4000-CN_TOUCH_FLAG_SIZE -CN_TOUCH_ADJUST_SIZE)
static const u8  buf_flag[CN_TOUCH_FLAG_SIZE]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
extern u32 AT24_ReadBytes(u32 wAddr, u8 *pbyBuf, u32 wBytesNum);
extern u32 AT24_WriteBytes(u32 wAddr, u8 *pbyBuf, u32 wBytesNum);

static  s16 gs_XadjustLeft=0,gs_XadjustRight=0;
static  s16 gs_YadjustTop=0,gs_YadjustBottom=0;

//定义SPIBUS架构下的SPI设备结构
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
// 功能：将新校准得到的校准值写进片上Flash的最后8个字节中。
// 参数：pAdjustValue,校准值结构体指针
// 返回：true,写成功;false,写失败
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
// 功能：读取触摸屏AD采样值，分别为X值和Y值，未做校正的数据
// 参数：X,Y，输出参数，存储读取到的数据地址
// 返回：true,读取成功;false,读取失败
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
    //读4个采样点的值
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
    //丢弃第一个采样点（不准），去除后面3个采样点，并提取最大最小值
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

    //最大最小值之差大于20的话，说明采样值不稳定（触笔在抖动）
    if( (maxx - minx > 20) || (maxy - miny > 20) )
        return false;
    else
    {
        for(loop = 1;loop<4;loop++)
        {
            tempx +=x0[loop];
            tempy +=y0[loop];
        }
        tempx -= minx;      //减掉最大值和最小值
        tempx -= maxx;
        tempy -= miny;
        tempy -= maxy;
        *X = tempx / 1;
        *Y = tempy / 1;
        return true;
    }
}

// =============================================================================
// 功能：读取此时是否触摸屏上有触摸
// 参数：无
// 返回：1,触摸;0,未触摸
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


//----读取触摸点坐标-----------------------------------------------------------
//功能: 读取stmpe811采集到的触摸点坐标，如果有多点，则平均之
//参数: touch_data，采集到的坐标
//返回: 1=触摸笔按下，0=触摸笔提起，
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

//----读取触摸点坐标-----------------------------------------------------------
//功能: 读取stmpe811采集到的触摸点坐标，如果有多点，则平均之
//参数: touch_data，采集到的坐标
//返回: 1=触摸笔按下，0=触摸笔提起，
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
                            //如果连续触摸且两 次采样之间移动距离达到6个像素，说明是提笔过程中的误读,不处理
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
    GK_ApiLineto(desktop,x,y-9,x,y+9,CN_COLOR_WHITE,CN_R2_COPYPEN,0); //上
    GK_ApiLineto(desktop,x-9,y,x+9,y,CN_COLOR_WHITE,CN_R2_COPYPEN,0);  //右
    GK_ApiSyncShow(1000*mS);
}

static  void clr_cursor(struct GkWinObj *desktop,s32 x,s32 y)
{
    GK_ApiLineto(desktop,x,y-9,x,y+9,CN_COLOR_BLACK,CN_R2_COPYPEN,0); //上
    GK_ApiLineto(desktop,x-9,y,x+9,y,CN_COLOR_BLACK,CN_R2_COPYPEN,0);  //右
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

    //先判断Flash中是否已存储有效的校准值   必须要校验
    if(i!=CN_TOUCH_FLAG_SIZE)
    {
        printf("\r\n");
        printf("LCM240128C TOUCH Adjust.");
        GK_ApiDrawText(desktop,NULL,NULL,40,20,
                        "触摸屏矫正 ",11,CN_COLOR_WHITE,CN_R2_COPYPEN,0);
        GK_ApiDrawText(desktop,NULL,NULL,40,40,
                        "请准确点击十字交叉点",20,CN_COLOR_WHITE,CN_R2_COPYPEN,0);
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


        //做容错处理
        if((s_s16gXAdjustLeft==s_s16gXAdjustRight)||(s_s16gYAdjustBottom==s_s16gYAdjustTop))
        {
            printf("TOUCH Adjust failed.TOUCH invalid.\r\n");
            return;
        }

        result=Touch_WriteAdjustValue(pAdjustValue);

        if(result)
        {
            GK_ApiDrawText(desktop,NULL,NULL,50,600,"校准成功",
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
//----初始化触摸屏模块---------------------------------------------------------
//功能:
//参数: display_dev_name:本触摸屏对应的显示器名(资源名)
//      touch_dev_name:触摸屏设备名.
//返回: 无
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

    //修改byzhb20170325   原来是1M改为500k，触摸点在边缘时不准。
    s_ptTouch_Dev = SPI_DevAdd(SPI_BUS_NAME,CFG_LCM240128_TOUCH_NAME,0,8,SPI_MODE_0,
                                SPI_SHIFT_MSB,500000,false)
    if(NULL != s_ptTouch_Dev)
    {
        SPI_BusCtrl(s_ptTouch_Dev,CN_SPI_SET_POLL,0,0);
    }
    else
    {
        printf("安装触摸屏SPI驱动出错\n\r");
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
    //事件的两个参数暂设为0?如果用shell启动?可用来采集shell命令行参数
    DJY_EventPop(evtt_id,NULL,0,0,0,0);

    return true;
}


