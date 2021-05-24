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
//所属模块:标准输入--触摸屏模块

//------------------------------------------------------

#include "stdint.h"
#include "stdio.h"
#include "stddef.h"
#include "Touch.h"
#include "string.h"
#include "gkernel.h"
//#include "cpu_peri_iic.h"
#include "iicbus.h"
//#include "ctiic.h"
#include "board.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_FT6236(void);
//    ModuleInstall_FT6236( );
//#if(CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT == true)
//    extern bool_t GDD_AddInputDev(const char *InputDevName);
//    GDD_AddInputDev(CFG_FT6236_TOUCH_NAME);
//#endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"touchscreen FT6236"//iic接口的触摸屏控制
//parent:"touch"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"io analog iic bus","touch"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"          //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_TOUCHSCREEN_FT6236 == false )
//#warning  " touchscreen_FT6236  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_TOUCHSCREEN_FT6236    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@enum,24,32,
#define  CFG_TOUCH_SIZE  32     //"触摸尺寸",
//%$#@enum,true,false,
#define  CFG_TOUCH_TURN  false          //"纵坐标反转",纵坐标是否需要反转
//%$#@num,1,100,
#define CT_MAX_TOUCH  1                         //"触控数",目前只支持单点触控,支持最多5点触摸
//%$#@enum,true,false,
//%$#@string,1,128,
#define CFG_TOUCH_ADJUST_FILE   "/efs/touch_init.dat"  //"矫正文件路径",保存触摸屏矫正参数的文件
#define CFG_FT6236_BUS_NAME     "IoIic"        //"IIC总线名称",触摸芯片使用的IIC总线名称
#define CFG_FT6236_TOUCH_NAME   "FT6236"       //"触摸屏名称",配置触摸屏名称
#define CFG_TARGET_DISPLAY_NAME "display"      //"触屏所在显示器名称",配置触摸屏所在显示器的名称
//%$#@select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#if (CFG_TOUCH_SIZE == 24)
#define FT_CMD_WR               (0X15<<1)
#define FT_CMD_RD               (0X15<<1|1)
#elif (CFG_TOUCH_SIZE == 32)
#define FT_CMD_WR               0X70        //写命令
#define FT_CMD_RD               0X71        //读命令
#endif
//FT6236 部分寄存器定义
#define FT_DEVIDE_MODE          0x00//FT6236模式控制寄存器
#define FT_REG_NUM_FINGER       0x02//触摸状态寄存器

#define FT_TP1_REG              0X03//第一个触摸点数据地址
#define FT_TP2_REG              0X09//第二个触摸点数据地址
#define FT_TP3_REG              0X0F//第三个触摸点数据地址
#define FT_TP4_REG              0X15//第四个触摸点数据地址
#define FT_TP5_REG              0X1B//第五个触摸点数据地址


#define FT_ID_G_LIB_VERSION     0xA1//版本
#define FT_ID_G_MODE            0xA4//FT6236中断模式控制寄存器
#define FT_ID_G_THGROUP         0x80//触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE    0x88//激活状态周期设置寄存器

const u16 FT6236_TPX_TBL[CT_MAX_TOUCH]={FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,\
                                        FT_TP4_REG,FT_TP5_REG};

//定义IICBUS架构下的IIC设备结构
static struct IIC_Device *ps_FT6236_Dev = NULL;
static u32 s_FT6236_Timeout = CN_TIMEOUT_FOREVER;
static struct SingleTouchPrivate FT6236;
static struct ST_TouchAdjust tg_touch_adjust;

//是否为竖屏标志位
static bool_t vertical_screen_flag = true;
__attribute__((weak))  void FT6236_Pin_Init(void)
{
    return;
}

// =============================================================================
// 功能：    读函数，
// 参数：    RegAddr 寄存器地址  num 数据长度
// 返回：接收的字
// =============================================================================
static bool_t FT6236_RD_Reg ( u16 reg,u8 *buf, u8 len)
{
    return IIC_Read(ps_FT6236_Dev,reg&0xff,buf,len,s_FT6236_Timeout);
}
// =============================================================================
// 功能：写寄存器
// 参数：DelDev,器件结构体指针
//       addr,发送地址，即指存储地址
//       buf,发送数据缓冲区指针
//       len,发送数据长度，字节单位
//       block_option,阻塞选项,true表示阻塞发送，false为非阻塞发送
//       timeout,超时时间，us
// =============================================================================
static  bool_t FT6236_WR_Reg (u16 reg, u8 *buf,u8 len)
{
   return  IIC_Write(ps_FT6236_Dev,reg,buf,len,true,s_FT6236_Timeout);
}
/* =============================================================================
 功能：FT6236芯片初始化，初始化和加载设备到对应的IIC总线.
 参数：无
 返回：true,成功;false,失败
 =============================================================================*/
static bool_t FT6236_Init( )
{
    u8 temp[2];
//    u16 chipid=0;
    FT6236_Pin_Init();

    FT_RST(0);    //复位
    DJY_DelayUs(50*mS);
    FT_RST(1);   //释放复位
    DJY_DelayUs(100*mS);

    CT_IIC_SCL(1);
    CT_IIC_SDA(1);
    DJY_DelayUs(10*mS);

//    chipid = 0;
//    do{
//        FT6236_RD_Reg(0xa3,&chipid,2);
//         i++;
//         DJY_DelayUs(50*mS);
//         if((chipid==0x06)||(chipid==0x36)||(chipid==0x64))
//            break;
//      } while(i<20);
#if 1
    temp[0]=0;
    temp[1]=0;

    FT6236_WR_Reg(FT_DEVIDE_MODE,temp,1);   //进入正常操作模式
//    FT6236_WR_Reg(FT_ID_G_MODE,temp,1);     //查询模式
//    temp[0]=22;                             //触摸有效值，22，越小越灵敏
//    FT6236_WR_Reg(FT_ID_G_THGROUP,temp,1);  //设置触摸有效值
//    temp[0]=12;                             //激活周期，不能小于12，最大14
//    FT6236_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1);
    //读取版本号，参考值：0x3003
    FT6236_RD_Reg(0xa6,&temp[0],2);
    //版本:0X3003/0X0001/0X0002
//    if((temp[0]==0X30&&temp[1]==0X03)||temp[1]==0X01||temp[1]==0X02)
//    {
        printf("CTP ID:%x\r\n",((u16)temp[0]<<8)+temp[1]);
        return true;
//    }
#endif
//    printf("Touch FT6236 Init ERROR.\r\n");
//    return false;
}

//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.
//返回值:当前触屏状态.
// 1=触摸笔按下，0=触摸笔提起，
static bool_t FT6236_Scan(struct SingleTouchMsg *touch_data)
{
    u8 buf[4],i,mode;
    s32 tch_x,tch_y;
    u8  contact=0;

//    if(FT_INT == 0)
    FT6236_RD_Reg(FT_REG_NUM_FINGER,&mode,1);//读取触摸点的状态

    if((mode==0)||mode>5)
        return 0;
    tch_x=0;    tch_y=0; touch_data->z=0;
    for(i=0;i<CT_MAX_TOUCH;i++)
    {
        FT6236_RD_Reg(FT6236_TPX_TBL[i],buf,4); //读取XY坐标值
        if( ((buf[0]&0XF0)==0X80) && ((buf[2]&0XF0)!=0Xf0))
        {
#if (CFG_TOUCH_SIZE == 24)
            if(vertical_screen_flag)
            {
                tch_x +=((s32)(buf[0]&0X0F)<<8)+buf[1];
                tch_y +=((s32)(buf[2]&0X0F)<<8)+buf[3];
            }
            else
            {
                tch_y +=((s32)(buf[0]&0X0F)<<8)+buf[1];
                tch_x +=((s32)(buf[2]&0X0F)<<8)+buf[3];
            }
#elif (CFG_TOUCH_SIZE == 32)
            if(vertical_screen_flag)
            {
                tch_x +=((s32)(buf[0]&0X0F)<<8)+buf[1];
                tch_y +=((s32)(buf[2]&0X0F)<<8)+buf[3];
            }
            else
            {
                tch_y +=((s32)(buf[0]&0X0F)<<8)+buf[1];
                tch_x +=((s32)(buf[2]&0X0F)<<8)+buf[3];
            }
#endif

            contact++;
        }
    }

    if(contact ==0)
        return 0;

    tch_x /=contact;       //平均多次座标值
    tch_y /=contact;

    if(tg_touch_adjust.ratio_x != 0)
    {
        touch_data->x = ((tch_x << 16)- tg_touch_adjust.offset_x)
                            / tg_touch_adjust.ratio_x;
    }
    else
    {
        touch_data->x = tch_x;
    }

    if(tg_touch_adjust.ratio_y != 0)
    {
        touch_data->y = ((tch_y << 16)- tg_touch_adjust.offset_y)
                            / tg_touch_adjust.ratio_y;
    }
    else
    {
        touch_data->y = tch_y;
    }
    touch_data->z=1;
//    printf("touch_data->x = %d\r\n",touch_data->x);
//    printf("touch_data->y = %d\r\n",touch_data->y);
    return 1;
}


//1触摸屏按下   0松开触摸屏
static bool_t touch_check(void)
{
    u8 i;
    struct SingleTouchMsg touch_xyz0;
    for(i=0;i<5;i++)
    {
        DJY_DelayUs(50*mS);
        if(FT6236_Scan(&touch_xyz0))//判断松手
            i=0;
    }
    return 0;
}
//----触摸屏校准---------------------------------------------------------------
//功能: 触摸屏的尺寸可能与液晶可显示区域不完全一致，安装也可能有偏差，需要计算
//      校准系数和偏移量。为获得更高精度，使用定点小数。
//参数: display_name,本触摸屏对应的显示器名(资源名)//用静态变量存储每次开机校准一次//
//返回: 无
//-----------------------------------------------------------------------------
static bool_t touch_ratio_adjust(struct GkWinObj *desktop)
{
    struct SingleTouchMsg touch_xyz0,touch_xyz1;
    s32 limit_left,limit_top,limit_right,limit_bottom;
    FILE *touch_init = NULL;
    touch_init = fopen(CFG_TOUCH_ADJUST_FILE,"r");
    if(touch_init != NULL)
    {
        fread(&tg_touch_adjust,sizeof(struct ST_TouchAdjust),1,touch_init);
        fclose(touch_init);
    }
    else
    {
        memset(&tg_touch_adjust, 0, sizeof(struct ST_TouchAdjust));
        limit_left = desktop->limit_left;
        limit_top = desktop->limit_top;
        limit_right = desktop->limit_right;
        limit_bottom = desktop->limit_bottom;
//
//        GK_FillWin(desktop,CN_COLOR_WHITE,0);
//        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
//                       "触摸屏矫正卡片", 21, CN_COLOR_BLACK+1, CN_R2_COPYPEN, 0);
//        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
//                       "请准确点击十字交叉点", 21, CN_COLOR_BLACK+1, CN_R2_COPYPEN, 0);
//        GK_Lineto(desktop,0,20,40,20,CN_COLOR_RED,CN_R2_COPYPEN,0);
//        GK_Lineto(desktop,20,0,20,40,CN_COLOR_RED,CN_R2_COPYPEN,CN_TIMEOUT_FOREVER);
//        GK_SyncShow(CN_TIMEOUT_FOREVER);
//        while(!FT6236_Scan(&touch_xyz0));//等待触摸
#if (CFG_TOUCH_SIZE == 24)
        if(vertical_screen_flag)
        {
            #if(CFG_TOUCH_TURN == false)
                touch_xyz0.x = 20;
                touch_xyz0.y = 20;
            #else
                touch_xyz0.x = 220;
                touch_xyz0.y = 300;
            #endif
        }
        else
        {
            #if(CFG_TOUCH_TURN == false)
                touch_xyz0.x = 300;
                touch_xyz0.y = 20;
            #else
                touch_xyz0.x = 20;
                touch_xyz0.y = 220;
            #endif
        }
#elif (CFG_TOUCH_SIZE == 32)
        if(vertical_screen_flag)
        {
            #if(CFG_TOUCH_TURN == false)
                touch_xyz0.x = 20;
                touch_xyz0.y = 20;
            #else
                touch_xyz0.x = 220;
                touch_xyz0.y = 300;
            #endif
        }
        else
        {
            #if(CFG_TOUCH_TURN == false)
                touch_xyz0.x = 300;
                touch_xyz0.y = 20;
            #else
                touch_xyz0.x = 20;
                touch_xyz0.y = 220;
            #endif
        }
#endif

        printf("采集坐标1:(%d,%d)\n\r",touch_xyz0.x,touch_xyz0.y);
//        touch_check();


//        GK_FillWin(desktop,CN_COLOR_WHITE,0);
//        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
//                       "触摸屏矫正", 21, CN_COLOR_BLACK + 1, CN_R2_COPYPEN, 0);
//        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
//                       "再次准确点击十字交叉点", 21, CN_COLOR_BLACK + 1, CN_R2_COPYPEN, 0);
//        GK_Lineto(desktop,limit_right-40,limit_bottom-20,
//                      limit_right,limit_bottom-20,
//                      CN_COLOR_RED,CN_R2_COPYPEN,0);
//        GK_Lineto(desktop,limit_right-20,limit_bottom-40,
//                      limit_right-20,limit_bottom,
//                      CN_COLOR_RED,CN_R2_COPYPEN,0);
//        GK_SyncShow(CN_TIMEOUT_FOREVER);
//
//        while(!FT6236_Scan(&touch_xyz1)); //记录触摸屏第二点校正值
#if (CFG_TOUCH_SIZE == 24)
        if(vertical_screen_flag)
        {
            #if(CFG_TOUCH_TURN == false)
                touch_xyz1.x = 220;
                touch_xyz1.y = 300;
            #else
                touch_xyz1.x = 20;
                touch_xyz1.y = 20;
            #endif
        }
        else
        {
            #if(CFG_TOUCH_TURN == false)
                touch_xyz1.x = 20;
                touch_xyz1.y = 220;
            #else
                touch_xyz1.x = 300;
                touch_xyz1.y = 20;
            #endif
        }
#elif (CFG_TOUCH_SIZE == 32)
        if(vertical_screen_flag)
        {
            #if(CFG_TOUCH_TURN == false)
                touch_xyz1.x = 220;
                touch_xyz1.y = 300;
            #else
                touch_xyz1.x = 20;
                touch_xyz1.y = 20;
            #endif
        }
        else
        {
            #if(CFG_TOUCH_TURN == false)
                touch_xyz1.x = 20;
                touch_xyz1.y = 220;
            #else
                touch_xyz1.x = 300;
                touch_xyz1.y = 20;
            #endif
        }
#endif

        printf("采集坐标2:(%d,%d)\n\r",touch_xyz1.x,touch_xyz1.y);
//        touch_check();

//        GK_FillWin(desktop,CN_COLOR_GREEN,0);
        tg_touch_adjust.ratio_x = ((touch_xyz1.x - touch_xyz0.x)<<16)
                        /(limit_right - limit_left -40);
        tg_touch_adjust.offset_x = (touch_xyz0.x<<16) - 20*tg_touch_adjust.ratio_x;
        tg_touch_adjust.ratio_y = ((touch_xyz1.y - touch_xyz0.y)<<16)
                        /(limit_bottom- limit_top-40);
        tg_touch_adjust.offset_y= (touch_xyz0.y<<16) - 20*tg_touch_adjust.ratio_y;
//        GK_FillWin(desktop,CN_COLOR_BLUE,0);
//        GK_SyncShow(CN_TIMEOUT_FOREVER);
        touch_init = fopen(CFG_TOUCH_ADJUST_FILE,"w+");
        if(touch_init)
        {
            fwrite(&tg_touch_adjust,sizeof(struct ST_TouchAdjust),1,touch_init);
            fclose(touch_init);
        }
        else
        {
            return false;
        }
    }
    return true;
}

// =============================================================================
// 功能：FT6236模块初始化，
// 参数：无
// 返回：true,成功;false,失败
// =============================================================================
bool_t ModuleInstall_FT6236(void)
{
    struct GkWinObj *desktop;
    bool_t result = false;
    static struct IIC_Device* s_FT6236_Dev;

    //添加FT6236到IIC总线
    s_FT6236_Dev = IIC_DevAdd(CFG_FT6236_BUS_NAME,"IIC_Dev_FT6236",FT_CMD_WR>>1,0,8);
    if(NULL != s_FT6236_Dev)
    {
        desktop = GK_GetDesktop(CFG_TARGET_DISPLAY_NAME);

        if((desktop->right-desktop->left) < (desktop->bottom-desktop->top))
            vertical_screen_flag=true;
        else
            vertical_screen_flag=false;

        IIC_BusCtrl(s_FT6236_Dev,CN_IIC_SET_CLK,100*1000,0);
        ps_FT6236_Dev = s_FT6236_Dev;
        result=FT6236_Init();
        if(!result)
            return false;

        result=touch_ratio_adjust(desktop);
        if(!result)
            return false;

        FT6236.read_touch = FT6236_Scan;    //读触摸点的坐标函数
        FT6236.touch_loc.display = GK_GetDisplay(CFG_TARGET_DISPLAY_NAME);
        result=Touch_InstallDevice(CFG_FT6236_TOUCH_NAME,&FT6236);//添加驱动到Touch
        if(!result)
            return false;

        result = true;
    }

    return result;
}


