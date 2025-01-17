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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "stdint.h"

#include "int_hard.h"
#include "int.h"
#include "cpu_peri.h"
#include "Touch.h"
#include "gkernel.h"
#include "systime.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_TOUCH_S3C2440(svoid);
//  ModuleInstall_TOUCH_S3C2440( );
//#if(CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT == true)
//    extern bool_t GDD_AddInputDev(const char *InputDevName);
//    GDD_AddInputDev(CFG_TOUCH_DEV_NAME);
//#endif
//%$#@describe      ****组件描述开始
//component name:"cpu onchip touch"//CPU触摸屏外设驱动
//parent:"touch"     //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                        //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                     //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                       //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                     //初始化时机，可选值：early，medium，later, pre-main。
                                       //表示初始化时间，分别是早期、中期、后期
//dependence:"graphical kernel","touch"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                       //选中该组件时，被依赖组件将强制选中，
                                       //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"gdd"                 //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                       //选中该组件时，被依赖组件不会被强制选中，
                                       //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                       //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_TOUCH == false )
//#warning  " cpu_onchip_touch  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_TOUCH    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_TOUCH_DEV_NAME               "TOUCH"   //"触摸设备名称",
#define CFG_DISPLAY_NAME                  "LCD"    //"触摸所在桌面的名称",
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

/*============================================================================*/
#define ADC_REG_BASE       (ADC_REG*)0x58000000

typedef struct ADC_REG{
    vu32 ADCCON;
    vu32 ADCTSC;
    vu32 ADCDLY;
    vu32 ADCDAT0;
    vu32 ADCDAT1;
    vu32 ADCUPDN;
}ADC_REG ;

#define ADC_MCLK    (50*1000*1000)
#define ADC_CLK     2000000 /*2MHZ*/

/*============================================================================*/

#define TS_XY_SWAP
#define TS_ORG_RD

#define TS_CAL_XOFFSET      (40)
#define TS_CAL_YOFFSET      (40)

#define TS_CAL_LU_XPOS      (TS_CAL_XOFFSET)
#define TS_CAL_LU_YPOS      (TS_CAL_YOFFSET)

#define TS_CAL_RU_XPOS      (CN_LCD_XSIZE-TS_CAL_XOFFSET)
#define TS_CAL_RU_YPOS      (TS_CAL_YOFFSET)

#define TS_CAL_LD_XPOS      (TS_CAL_XOFFSET)
#define TS_CAL_LD_YPOS      (CN_LCD_YSIZE-TS_CAL_YOFFSET)

#define TS_CAL_RD_XPOS      (CN_LCD_XSIZE-TS_CAL_XOFFSET)
#define TS_CAL_RD_YPOS      (CN_LCD_YSIZE-TS_CAL_YOFFSET)

#define TS_CAL_XSIZE        (TS_CAL_RU_XPOS-TS_CAL_LU_XPOS)
#define TS_CAL_YSIZE        (TS_CAL_RD_YPOS-TS_CAL_RU_YPOS)

typedef struct TS_CFG_DATA
{
    u16 LUAdx,LUAdy;
    u16 RUAdx,RUAdy;
    u16 RDAdx,RDAdy;
    u16 LDAdx,LDAdy;
}TS_CFG_DATA;

///////////////////

static  int ts_xsize,ts_ysize;
static  int ts_cal_ref_pos[4][2];
static  TS_CFG_DATA ts_cfg_data;   //配置数据

#define CN_LCD_XSIZE   ts_xsize
#define CN_LCD_YSIZE   ts_ysize

/*============================================================================*/
/*============================================================================*/

static bool_t ts_translate_data(s32 *x,s32 *y)
{
    int x_coor,y_coor,adx,ady;
    int ts_adx_min,ts_adx_max,ts_ady_min,ts_ady_max;

    if((x!=NULL)&&(y!=NULL))
    {
        adx =*x;
        ady =*y;

#ifdef  TS_ORG_LU
        ts_adx_min  =ts_cfg_data.LUAdx;
        ts_ady_min  =ts_cfg_data.LUAdy;

        ts_adx_max  =ts_cfg_data.RDAdx;
        ts_ady_max  =ts_cfg_data.RDAdy;

        if((ts_adx_min==ts_adx_max) || (ts_ady_min==ts_ady_max))
        {
            *x  =-1;
            *y  =-1;
            return  false;
        }

#ifdef  TS_XY_SWAP
        x_coor = TS_CAL_XOFFSET+(CN_LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
        y_coor = TS_CAL_YOFFSET+(CN_LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
#else
        x_coor = TS_CAL_XOFFSET+(CN_LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
        y_coor = TS_CAL_YOFFSET+(CN_LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
#endif
        *x =x_coor;
        *y =y_coor;
#endif

#ifdef  TS_ORG_RU
        ts_adx_min  =ts_cfg_data.RUAdx;
        ts_ady_min  =ts_cfg_data.RUAdy;

        ts_adx_max  =ts_cfg_data.LDAdx;
        ts_ady_max  =ts_cfg_data.LDAdy;

        if((ts_adx_min==ts_adx_max) || (ts_ady_min==ts_ady_max))
        {
            *x  =-1;
            *y  =-1;
            return  false;
        }

#ifdef  TS_XY_SWAP
        x_coor = TS_CAL_XOFFSET+(CN_LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
        y_coor = TS_CAL_YOFFSET+(CN_LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
#else
        x_coor = TS_CAL_XOFFSET+(CN_LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
        y_coor = TS_CAL_YOFFSET+(CN_LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
#endif
        *x  =CN_LCD_XSIZE-x_coor;
        *y  =y_coor;
#endif

#ifdef  TS_ORG_RD
        ts_adx_min  =ts_cfg_data.RDAdx;
        ts_ady_min  =ts_cfg_data.RDAdy;

        ts_adx_max  =ts_cfg_data.LUAdx;
        ts_ady_max  =ts_cfg_data.LUAdy;

        if((ts_adx_min==ts_adx_max) || (ts_ady_min==ts_ady_max))
        {
            *x  =-1;
            *y  =-1;
            return  false;
        }

#ifdef  TS_XY_SWAP
        x_coor = TS_CAL_XOFFSET+(CN_LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
        y_coor = TS_CAL_YOFFSET+(CN_LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
#else
        x_coor = TS_CAL_XOFFSET+(CN_LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
        y_coor = TS_CAL_YOFFSET+(CN_LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
#endif
        *x  =CN_LCD_XSIZE-x_coor;
        *y  =CN_LCD_YSIZE-y_coor;
#endif

#ifdef  TS_ORG_LD
        ts_adx_min  =ts_cfg_data.LDAdx;
        ts_ady_min  =ts_cfg_data.LDAdy;

        ts_adx_max  =ts_cfg_data.RUAdx;
        ts_ady_max  =ts_cfg_data.RUAdy;

        if((ts_adx_min==ts_adx_max) || (ts_ady_min==ts_ady_max))
        {
            *x  =-1;
            *y  =-1;
            return  false;
        }

#ifdef  TS_XY_SWAP
        x_coor = TS_CAL_XOFFSET+(CN_LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
        y_coor = TS_CAL_YOFFSET+(CN_LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
#else
        x_coor = TS_CAL_XOFFSET+(CN_LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
        y_coor = TS_CAL_YOFFSET+(CN_LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
#endif
        *x  =x_coor;
        *y  =CN_LCD_YSIZE-y_coor;

#endif
        return true;
    }
    return  false;
}

/*============================================================================*/


void touch_ratio_adjust(struct GkWinObj *desktop);
static ufast_t read_touch_data(struct SingleTouchMsg *touch_data);
static bool_t touch_hard_init(void);

//----初始化触摸屏模块---------------------------------------------------------
//功能:
//参数: display_dev_name:本触摸屏对应的显示器名(资源名)
//      touch_dev_name:触摸屏设备名.
//返回: 无
//-----------------------------------------------------------------------------
bool_t ModuleInstall_TOUCH_S3C2440(void)
{
    struct GkWinObj *desktop;
    static struct SingleTouchPrivate touch_dev;

    desktop = GK_GetDesktop(CFG_DISPLAY_NAME);
    ts_xsize =desktop->disp->width;
    ts_ysize = desktop->disp->height;

    ts_cal_ref_pos[0][0] =TS_CAL_LU_XPOS;
    ts_cal_ref_pos[0][1] =TS_CAL_LU_YPOS;
    ts_cal_ref_pos[1][0] =TS_CAL_RU_XPOS;
    ts_cal_ref_pos[1][1] =TS_CAL_RU_YPOS;
    ts_cal_ref_pos[2][0] =TS_CAL_RD_XPOS;
    ts_cal_ref_pos[2][1] =TS_CAL_RD_YPOS;
    ts_cal_ref_pos[3][0] =TS_CAL_LD_XPOS;
    ts_cal_ref_pos[3][1] =TS_CAL_LD_YPOS;

    if(!touch_hard_init( ))
    {
        return false;
    }
    touch_ratio_adjust(desktop);
    touch_dev.read_touch = read_touch_data;
    touch_dev.touch_loc.display = GK_GetDisplay(CFG_DISPLAY_NAME);
    Touch_InstallDevice(CFG_TOUCH_DEV_NAME,&touch_dev);
    return true;
}

//----触摸屏硬件初始化---------------------------------------------------------
//功能: 触摸屏硬件初始化
//参数: 无
//返回: 键盘设备号，-1表示按照键盘设备失败
//-----------------------------------------------------------------------------
static bool_t touch_hard_init(void)
{
    int prs;

    ADC_REG *adc=ADC_REG_BASE;

    prs = (ADC_MCLK/ADC_CLK)-1;

    adc->ADCDLY = 4000;                  //Normal conversion mode delay about (1/3.6864M)*50000=13.56ms
    adc->ADCCON = (1<<14) | (prs<<6) | (1<<3)| (0<<2)  ;       //read start
    adc->ADCTSC = 0xd3;  //Wfait,XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En
    return true;
}

static  bool_t ts_is_down(void)
{
    ADC_REG *adc=ADC_REG_BASE;
    int i,j;
    ///

    for(i=0;i<4;i++)    //检测4次
    {
        if((adc->ADCDAT0&(1<<15)))  return  false;
        if((adc->ADCDAT1&(1<<15)))  return  false;
        for(j=0;j<50;j++);  //Delay
    }
    //DebugPrintf("ts_down\r\n");
    return  true;
}

static  void InsertSort(int *A, int p, int r)
{
    int i,j;
    int key;
    for(i=p+1; i<=r; i++)
    {
        key = A[i];
        j = i-1;
        while (j >= 0 && A[j] > key)
        {
            A[j+1] = A[j];
            j--;
        }
        A[j+1] = key;
    }
}

#define TS_SAMPLE_COUNT 8
static  int ad_buf[TS_SAMPLE_COUNT];

static  int ts_get_x_data_raw(void)
{
    int i,j,prs;
    int dat;
    ADC_REG *adc=ADC_REG_BASE;
    /////

    prs  = (ADC_MCLK/ADC_CLK)-1;

    adc->ADCDLY = 1000;
    adc->ADCCON = (1<<14) | (prs<<6)|(1<<3)  ;     //read start
    adc->ADCTSC = (0xF<<4)|(1<<3)|(0<<2)|(1);// read x
    for(j=200;j>0;j--);

    for(i=0;i<TS_SAMPLE_COUNT;i++)
    {
        adc->ADCCON |=0x1;                  //start ADC
        for(j=200;j>0;j--);
        while(adc->ADCCON & 0x1);           //check if Enable_start is low
        while(!(adc->ADCCON & (1<<15)));    //check if EC(End of Conversion) flag is high, This line is necessary~!!

        ad_buf[i]=adc->ADCDAT0&0x3FF;

    }

    adc->ADCTSC = 0xd3;

    InsertSort(ad_buf,0,TS_SAMPLE_COUNT-1);
    i = TS_SAMPLE_COUNT>>1;
    dat = 0;
    dat += ad_buf[i++];
    dat += ad_buf[i++];
    dat = dat>>1;

//  DebugPrintf("ts_adx =%04XH\r\n",dat);

    return dat;
    ////
}

static  int ts_get_y_data_raw(void)
{
    int i,j,prs;
    int dat;
    ADC_REG *adc=ADC_REG_BASE;
    /////

    prs  = (ADC_MCLK/ADC_CLK)-1;

    adc->ADCDLY = 1000;
    adc->ADCCON = (1<<14) | (prs<<6)|(1<<3)  ;     //read start
    adc->ADCTSC = (0xF<<4)|(1<<3)|(0<<2)|(2);// read y
    for(j=200;j>0;j--);

    for(i=0;i<TS_SAMPLE_COUNT;i++)
    {
        adc->ADCCON |=0x1;                  //start ADC
        for(j=200;j>0;j--);
        while(adc->ADCCON & 0x1);           //check if Enable_start is low
        while(!(adc->ADCCON & (1<<15)));    //check if EC(End of Conversion) flag is high, This line is necessary~!!

        ad_buf[i] =adc->ADCDAT1&0x3FF;


    }

    adc->ADCTSC = 0xd3;

    InsertSort(ad_buf,0,TS_SAMPLE_COUNT-1);
    i = TS_SAMPLE_COUNT>>1;
    dat = 0;
    dat += ad_buf[i++];
    dat += ad_buf[i++];
    dat = dat>>1;

//  DebugPrintf("ts_ady =%04XH\r\n",dat);

    return dat;
    ////

}

static bool_t   ts_get_data_raw(s32 *x,s32 *y)
{
    if(ts_is_down())
    {
        x=ts_get_x_data_raw();
        y=ts_get_y_data_raw();
        return true;
    }
    return false;
}

//----读取触摸点坐标-----------------------------------------------------------
//功能: 读取stmpe811采集到的触摸点坐标，如果有多点，则平均之
//参数: touch_data，采集到的坐标
//返回: 1=触摸笔按下，0=触摸笔提起，
//-----------------------------------------------------------------------------
static ufast_t read_touch_data(struct SingleTouchMsg *touch_data)
{
     static s32 x=0,y=0,z=0;

    touch_data->display = NULL;
    if(ts_is_down())
    {

        x =ts_get_x_data_raw();
        y =ts_get_y_data_raw();
        z =1;
        ts_translate_data(&x,&y);
        touch_data->x =x;
        touch_data->y =y;
        touch_data->z = z;

        return 1;
    }
    else
    {
        touch_data->z = 0;
        return 0;
    }

}


static  void draw_cursor(struct GkWinObj *desktop,int x,int y)
{
    GK_Lineto(desktop,NULL,x,y,x,y-20+1,CN_COLOR_RED,CN_R2_COPYPEN,0); //上
    GK_Lineto(desktop,NULL,x,y,x,y+20-1,CN_COLOR_RED,CN_R2_COPYPEN,0); //下
    GK_Lineto(desktop,NULL,x,y,x+20-1,y,CN_COLOR_RED,CN_R2_COPYPEN,0);//右
    GK_Lineto(desktop,NULL,x,y,x-20+1,y,CN_COLOR_RED,CN_R2_COPYPEN,0); //左
    GK_SyncShow(1000*mS);
}

static  void clr_cursor(struct GkWinObj *desktop,int x,int y)
{
    GK_Lineto(desktop,NULL,x,y,x,y-20+1,CN_COLOR_WHITE,CN_R2_COPYPEN,0); //上
    GK_Lineto(desktop,NULL,x,y,x,y+20-1,CN_COLOR_WHITE,CN_R2_COPYPEN,0); //下
    GK_Lineto(desktop,NULL,x,y,x+20-1,y,CN_COLOR_WHITE,CN_R2_COPYPEN,0);//右
    GK_Lineto(desktop,NULL,x,y,x-20+1,y,CN_COLOR_WHITE,CN_R2_COPYPEN,0); //左
    GK_SyncShow(1000*mS);

}

//----触摸屏校准---------------------------------------------------------------
//功能: 触摸屏的尺寸可能与液晶可显示区域不完全一致，安装也可能有偏差，需要计算
//      校准系数和偏移量。为获得更高精度，使用定点小数。
//参数: display_name,本触摸屏对应的显示器名(资源名)
//返回: 无
//-----------------------------------------------------------------------------
void touch_ratio_adjust(struct GkWinObj *desktop)
{

    struct SingleTouchMsg touch_xyz0,touch_xyz1;
    FILE *touch_init;
    s32 limit_left,limit_top,limit_right,limit_bottom,pen_down_time,step;

    if((touch_init = fopen("sys:\\touch_init.dat","r")) != NULL)
    {

        fread(&ts_cfg_data,sizeof(TS_CFG_DATA),1,touch_init);
    }
    else
    {
        limit_left = desktop->limit.left;
        limit_top = desktop->limit.top;
        limit_right = desktop->limit.right;
        limit_bottom = desktop->limit.bottom;
    //    GK_CreateWin(desktop,desktop,limit_left,limit_top,limit_right,limit_bottom,
    //                      CN_COLOR_WHITE,CN_WINBUF_BUF,"&tg_touch_adjust",CN_R3_SRCCOPY,0);
    //    GK_SetPrio(desktop,-1,CN_GK_SYNC);
        GK_FillWin(desktop,CN_COLOR_WHITE,0);
        GK_DrawText(desktop,NULL,NULL,NULL,limit_left+10,limit_top+50,
                            "触摸屏矫正",21,CN_COLOR_WHITE,CN_R2_COPYPEN,0);
        GK_DrawText(desktop,NULL,NULL,NULL,limit_left+10,limit_top+70,
                            "请准确点击十字交叉点",21,CN_COLOR_WHITE,CN_R2_COPYPEN,0);

        step=0;
        while(step<4)
        {
            s32 adx,ady;

            if(step>0)
            {
                clr_cursor(desktop,ts_cal_ref_pos[step-1][0],ts_cal_ref_pos[step-1][1]);
            }
            draw_cursor(desktop,ts_cal_ref_pos[step][0],ts_cal_ref_pos[step][1]);

            while(ts_is_down())
            {
                DJY_DelayUs(100*mS);
            }
            pen_down_time=0;
            while(1)
            {
                if(ts_is_down())
                {

                    adx=ts_get_x_data_raw();
                    ady=ts_get_y_data_raw();
                    if(pen_down_time++>5)
                    {
                        break;
                    }
                }
                else
                {
                    pen_down_time=0;
                }
                DJY_DelayUs(100*mS);
            }

            switch(step)
            {
            case 0:
                ts_cfg_data.LUAdx =adx;
                ts_cfg_data.LUAdy =ady;
                break;

            case 1:
                ts_cfg_data.RUAdx =adx;
                ts_cfg_data.RUAdy =ady;
                break;

            case 2:
                ts_cfg_data.RDAdx =adx;
                ts_cfg_data.RDAdy =ady;
                break;

            case 3:
                ts_cfg_data.LDAdx =adx;
                ts_cfg_data.LDAdy =ady;
                break;

            }
            printf("ts_cal[%d]: %04X,%04X\r\n",step,adx,ady);
            step++;
        }

        GK_FillWin(desktop,CN_COLOR_WHITE,0);
        GK_SyncShow(1000*mS);
        while(ts_is_down())
        {
            DJY_DelayUs(100*mS);
        }
        DJY_DelayUs(500*mS);

    //    GK_DestroyWin(desktop);
        touch_init = fopen("sys:\\touch_init.dat","w+");
        fwrite(&ts_cfg_data,sizeof(TS_CFG_DATA),1,touch_init);
    }
    fclose(touch_init);

}
