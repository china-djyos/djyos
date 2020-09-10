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
//    extern bool_t ModuleInstall_GT911(void);
//    ModuleInstall_GT911( );
//#if(CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT == true)
//    extern bool_t GDD_AddInputDev(const char *InputDevName);
//    GDD_AddInputDev(CFG_GT911_TOUCH_NAME);
//#endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"touchscreen GT911"//iic接口的触摸屏控制
//parent:"touch"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:pre-main              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"cpu onchip iic","touch"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"          //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_TOUCHSCREEN_GT911 == false )
//#warning  " touchscreen_GT911  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_TOUCHSCREEN_GT911    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@enum,true,false,
//%$#@string,1,128,
#define CFG_GT911_BUS_NAME     "IIC1"        //"IIC总线名称",触摸芯片使用的IIC总线名称
#define CFG_GT911_TOUCH_NAME   "gt911"       //"触摸屏名称",配置触摸屏名称
#define CFG_TARGET_DISPLAY_NAME "LCD_F7"      //"触屏所在显示器名称",配置触摸屏所在显示器的名称
//%$#@select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


extern void Set_TpINT_ToIn(void); //在board.c里实现，设置TP的INT脚为输入模式

#define GT911_ADDR                0xBA
#define GT911_ADDR_BITS           16

#define GT911_COMMAND             0x8040
#define GT911_CONFIG_VERSION      0x8047
#define GT911_CONFIG_CHKSUM       0x80FF
#define GT911_PRODUCT_ID          0x8140
#define GT911_SENSOR_ID           0x814A
#define GT911_READ_COOR_ADDR      0x814E

#define GT911_TP1_REG              0X8150  //第一个触摸点数据地址
#define GT911_TP2_REG              0X8158  //第二个触摸点数据地址
#define GT911_TP3_REG              0X8160  //第三个触摸点数据地址
#define GT911_TP4_REG              0X8168  //第四个触摸点数据地址
#define GT911_TP5_REG              0X8170  //第五个触摸点数据地址
#define GT911_TP6_REG              0X8178  //第六个触摸点数据地址
#define GT911_TP7_REG              0X8180  //第七个触摸点数据地址
#define GT911_TP8_REG              0X8188  //第八个触摸点数据地址
#define GT911_TP9_REG              0X8190  //第九个触摸点数据地址
#define GT911_TP10_REG             0X8198  //第十个触摸点数据地址
//#define GT911_TP1_REG              0X814f  //第一个触摸点track id地址
//#define GT911_TP2_REG              0X8157  //第二个触摸点track id地址
//#define GT911_TP3_REG              0X815f  //第三个触摸点track id地址
//#define GT911_TP4_REG              0X8167  //第四个触摸点track id地址
//#define GT911_TP5_REG              0X816f  //第五个触摸点track id地址
//#define GT911_TP6_REG              0X8177  //第六个触摸点track id地址
//#define GT911_TP7_REG              0X817f  //第七个触摸点track id地址
//#define GT911_TP8_REG              0X8187  //第八个触摸点track id地址
//#define GT911_TP9_REG              0X818f  //第九个触摸点track id地址
//#define GT911_TP10_REG             0X8197  //第十个触摸点track id地址

#define GT911_MAX_TOUCH           0x0A         //等于CTP_CFG_GROUP5中的第6个元素



const u16 GT911_TPX_TBL[]={GT911_TP1_REG, GT911_TP2_REG, GT911_TP3_REG,
        GT911_TP4_REG, GT911_TP5_REG, GT911_TP6_REG, GT911_TP7_REG, GT911_TP8_REG,
        GT911_TP9_REG, GT911_TP10_REG};

//定义IICBUS架构下的IIC设备结构
static struct IIC_Device *ps_GT911_Dev = NULL;
static u32 s_GT911_Timeout = CN_TIMEOUT_FOREVER;
static struct SingleTouchPrivate GT911;
__attribute__((weak))  void GT911_Pin_Init(void)
{
    return;
}

#if 0
/* TODO: define your config for Sensor_ID == 5 here, if needed */
#define CTP_CFG_GROUP5 {\
    0x5c,0x00,0x04,0x58,0x02,0x0A,0x3D,0x00,0x01,0x08,0x28,0x0F,0x50,0x32,0x03,0x05,\
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x17,0x19,0x1D,0x14,0x90,0x2F,0x89,0x1E,0x20,\
    0x31,0x0D,0x00,0x00,0x00,0x43,0x03,0x25,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x14,0x2D,0x94,0xD5,0x02,0x07,0x00,0x00,0x04,0xAF,0x15,0x00,0x95,\
    0x19,0x00,0x80,0x1E,0x00,0x70,0x23,0x00,0x63,0x29,0x00,0x63,0x00,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
    0x16,0x15,0x14,0x11,0x10,0x0F,0x0E,0x0D,0x0C,0x09,0x08,0x07,0x06,0x05,0x04,0x01,\
    0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x29,0x28,\
    0x27,0x26,0x25,0x24,0x23,0x22,0x21,0x20,0x1F,0x1E,0x1C,0x1B,0x19,0x14,0x13,0x12,\
    0x11,0x10,0x0F,0x0E,0x0D,0x0C,0x0A,0x08,0x07,0x06,0x04,0x02,0x00,0xFF,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF3,0x01\
}
#else
#define CTP_CFG_GROUP5 {\
    0x65,0x00,0x04,0x58,0x02,0x0A,0x3D,0x00,0x01,0x08,0x28,0x0F,0x50,0x32,0x03,\
    0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x17,0x19,0x1D,0x14,0x90,0x2F,0x89,\
    0x17,0x19,0x31,0x0D,0x00,0x00,0x00,0x01,0x03,0x2D,0x00,0x00,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x37,0x94,0xD5,0x02,0x07,0x00,0x00,0x04,\
    0xA2,0x11,0x00,0x90,0x16,0x00,0x80,0x1C,0x00,0x73,0x25,0x00,0x68,0x30,0x00,\
    0x68,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x16,0x15,0x14,0x11,0x10,0x0F,0x0E,0x0D,\
    0x0C,0x09,0x08,0x07,0x06,0x05,0x04,0x01,0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x29,0x28,0x27,0x26,0x25,0x24,0x23,0x22,\
    0x21,0x20,0x1F,0x1E,0x1C,0x1B,0x19,0x14,0x13,0x12,0x11,0x10,0x0F,0x0E,0x0D,\
    0x0C,0x0A,0x08,0x07,0x06,0x04,0x02,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,\
    0x00,0x00,0x00,0x00,0x32,0x01\
}
#endif
u8 cfg_info_group5[] = CTP_CFG_GROUP5;


// =============================================================================
// 功能：    读函数，
// 参数：    reg: 寄存器地址 ; buf: 数据缓存; len: 数据长度
// 返回：    接收的长度
// =============================================================================
static bool_t GT911_RD_Reg ( u16 reg,u8 *buf, u8 len)
{
    return IIC_Read(ps_GT911_Dev, reg&0xffff, buf, len, s_GT911_Timeout);
}
// =============================================================================
// 功能：  写寄存器
// 参数：  reg: 寄存器地址 ; buf: 数据缓存; len: 数据长度
// 返回：  写的长度
// =============================================================================
static  bool_t GT911_WR_Reg (u16 reg, u8 *buf,u8 len)
{
   return  IIC_Write(ps_GT911_Dev, reg, buf, len, true, s_GT911_Timeout);
}

/* =============================================================================
 功能：GT911芯片初始化，初始化和加载设备到对应的IIC总线.
 参数：无
 返回：true,成功;false,失败
 =============================================================================*/
static bool_t GT911_Init( )
{
    u8 temp[6];
//    u8 test_buf[512];
    u8 count = 0, Val_1 = 0, Val_2 = 0;
    s8 ret = 0;
    GPIO_SettoLow(GPIO_D,PIN2);         //RST 0
    DJY_EventDelay(20000);
    GPIO_SettoLow(GPIO_D,PIN3);         //INT 0
    DJY_EventDelay(2000);
    GPIO_SettoHigh(GPIO_D,PIN2);        //RST 1
    DJY_EventDelay(6000);
    GPIO_SettoLow(GPIO_D,PIN3);         //INT 0
    DJY_EventDelay(50000);

    Set_TpINT_ToIn();

    ret = GT911_RD_Reg(GT911_PRODUCT_ID, temp, 6);
    if (ret == 6)
    {
        if(temp[3] == 0x00)
            printf("IC Version: %c%c%c_%02X%02X.\r\n",temp[0], temp[1], temp[2], temp[5], temp[4]);
        else
            printf("IC Version: %c%c%c%c_%02X%02X.\r\n",temp[0], temp[1], temp[2], temp[3], temp[5], temp[4]);

        temp[4] = 0;
        if(strcmp((char*)temp, "911") == 0)
        {
            temp[0] = 2;
            GT911_WR_Reg(GT911_COMMAND, &temp[0], 1);
            ret = GT911_RD_Reg(GT911_SENSOR_ID, &Val_1, 1);
            ret = GT911_RD_Reg(GT911_SENSOR_ID, &Val_2, 1);
            if((ret == 1) && (Val_1 == Val_2))
            {
                printf(" Sensor_ID = %d.\r\n", Val_1);
                if(Val_1 < 0x06)
                {
                    return true;

//                    ret = GT911_RD_Reg(GT911_CONFIG_VERSION, test_buf, sizeof(cfg_info_group5));
//                    ret = GT911_RD_Reg(GT911_CONFIG_VERSION, &Val_2, 1);
//                    if((ret == 1) && (Val_2 >= 90))
//                    {
//                        if(GT911_WR_Reg(GT911_CONFIG_VERSION, cfg_info_group5, sizeof(cfg_info_group5)) == sizeof(cfg_info_group5))
//                        {
//                            Djy_EventDelay(2000*1000);
//                            ret = GT911_RD_Reg(GT911_CONFIG_VERSION, test_buf, sizeof(cfg_info_group5));
//                            if((ret > 0) && (test_buf[0] != cfg_info_group5[0]))    //用以判断配置信息是否真的写进去了。
//                                printf("config info write fail\r\n");
//                            for(count=0; count<sizeof(cfg_info_group5); count++)
//                                temp[0] += cfg_info_group5[count]; /*计算校验和*/
//                            temp[0] = (~temp[0]) + 1;
//                            temp[1] = 1;    //0,参数不保存到flash;1,参数保存到flash
//                            if(GT911_WR_Reg(GT911_CONFIG_CHKSUM, temp, 2) == 2)
//                                return true;
//                        }
//                    }
                }
            }
        }

    }
    return false;
}

//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.
//返回值:当前触屏状态.
// 1=触摸笔按下，0=触摸笔提起，
static bool_t GT911_Scan(struct SingleTouchMsg *touch_data)
{
    u8 touch_num, buf[4], mode;
    u8 i;
//    if(FT_INT == 0)
//    GT911_RD_Reg(FT_REG_NUM_FINGER,&mode,1);//读取触摸点的状态
    for(i = 0; i < 5; i++)
    {
        if(GT911_RD_Reg(GT911_READ_COOR_ADDR, &mode, 1))
        {
            if(mode & (1 << 7))     //读取0x814E一直为80表示抬起状态.bit7位为0的话，表示buffer没有准备好请延时1ms~10ms再读取,如果读取值是1表示buffer准备OK
            {
                touch_num = mode&0x0f;  //0~3位。表示touch number个数，为0表示抬起
                if(touch_num > 0)
                {
                    touch_data->z=0;
            //        for(i=0;i<GT911_MAX_TOUCH;i++)
            //        {
            //           GT911_RD_Reg(GT911_TPX_TBL[i],buf,4); //读取XY坐标值
            //           if( ((buf[0]&0XF0)==0X80) && ((buf[2]&0XF0)!=0Xf0))
            //           {
            //               tch_x +=((s32)(buf[0]&0X0F)<<8)+buf[1];
            //               tch_y +=((s32)(buf[2]&0X0F)<<8)+buf[3];
            //               contact++;
            //           }
            //        }
            //
            //        if(contact ==0)
            //            return 0;
            //
            //        tch_x /=contact;       //平均多次座标值
            //        tch_y /=contact;
                    GT911_RD_Reg(GT911_TPX_TBL[0],buf,4); //读取XY坐标值,只读触控点1的
                    touch_data->x = buf[0] | (buf[1] << 8);
                    touch_data->y = buf[2] | (buf[3] << 8);;
                    touch_data->z=1;

                    touch_num = 0;    //读取对应的坐标点后对0x814E清0
                    GT911_WR_Reg(GT911_READ_COOR_ADDR,&touch_num,1);
                //    printf("touch_data->x = %d\r\n",touch_data->x);
                //    printf("touch_data->y = %d\r\n",touch_data->y);
                    return 1;
                }
                touch_num = 0;  //0的话直接对0x814E清0
                GT911_WR_Reg(GT911_READ_COOR_ADDR,&touch_num,1);
            }
        }
        DJY_EventDelay(6000);
    }

    return 0;
}

#if 0
//1触摸屏按下   0松开触摸屏
static bool_t touch_check(void)
{
    u8 i;
    struct SingleTouchMsg touch_xyz0;
    for(i=0;i<5;i++)
    {
        Djy_DelayUs(50*mS);
        if(GT911_Scan(&touch_xyz0))//判断松手
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

        GK_FillWin(desktop,CN_COLOR_WHITE,0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                       "触摸屏矫正卡片", 21, CN_COLOR_BLACK+1, CN_R2_COPYPEN, 0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                       "请准确点击十字交叉点", 21, CN_COLOR_BLACK+1, CN_R2_COPYPEN, 0);
        GK_Lineto(desktop,0,20,40,20,CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_Lineto(desktop,20,0,20,40,CN_COLOR_RED,CN_R2_COPYPEN,CN_TIMEOUT_FOREVER);
        GK_SyncShow(CN_TIMEOUT_FOREVER);
        while(!GT911_Scan(&touch_xyz0));//等待触摸
        touch_xyz0.x = 220;
        touch_xyz0.y = 20;
        printf("采集坐标1:(%d,%d)\n\r",touch_xyz0.x,touch_xyz0.y);
        touch_check();


        GK_FillWin(desktop,CN_COLOR_WHITE,0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                       "触摸屏矫正", 21, CN_COLOR_BLACK + 1, CN_R2_COPYPEN, 0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                       "再次准确点击十字交叉点", 21, CN_COLOR_BLACK + 1, CN_R2_COPYPEN, 0);
        GK_Lineto(desktop,limit_right-40,limit_bottom-20,
                      limit_right,limit_bottom-20,
                      CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_Lineto(desktop,limit_right-20,limit_bottom-40,
                      limit_right-20,limit_bottom,
                      CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_SyncShow(CN_TIMEOUT_FOREVER);

        while(!GT911_Scan(&touch_xyz1)); //记录触摸屏第二点校正值
        touch_xyz1.x = 20;
        touch_xyz1.y = 300;
        printf("采集坐标2:(%d,%d)\n\r",touch_xyz1.x,touch_xyz1.y);
        touch_check();

        GK_FillWin(desktop,CN_COLOR_GREEN,0);
        tg_touch_adjust.ratio_x = ((touch_xyz1.x - touch_xyz0.x)<<16)
                        /(limit_right - limit_left -40);
        tg_touch_adjust.offset_x = (touch_xyz0.x<<16) - 20*tg_touch_adjust.ratio_x;
        tg_touch_adjust.ratio_y = ((touch_xyz1.y - touch_xyz0.y)<<16)
                        /(limit_bottom- limit_top-40);
        tg_touch_adjust.offset_y= (touch_xyz0.y<<16) - 20*tg_touch_adjust.ratio_y;
        GK_FillWin(desktop,CN_COLOR_BLUE,0);
        GK_SyncShow(CN_TIMEOUT_FOREVER);
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
#endif
// =============================================================================
// 功能：GT911模块初始化，
// 参数：无
// 返回：true,成功;false,失败
// =============================================================================
bool_t ModuleInstall_GT911(void)
{
    bool_t result = false;
    static struct IIC_Device* s_GT911_Dev;

    //添加GT911到IIC总线
    s_GT911_Dev = IIC_DevAdd(CFG_GT911_BUS_NAME,"IIC_Dev_GT911", GT911_ADDR >> 1, 0, GT911_ADDR_BITS);
    if(NULL != s_GT911_Dev)
    {
        GK_GetDesktop(CFG_TARGET_DISPLAY_NAME);
        IIC_BusCtrl(s_GT911_Dev,CN_IIC_SET_CLK,100*1000,0);
//        IIC_BusCtrl(s_GT911_Dev,CN_IIC_SET_POLL,0,0);
        ps_GT911_Dev = s_GT911_Dev;
        result=GT911_Init();
        if(!result)
            return false;

        GT911.read_touch = GT911_Scan;    //读触摸点的坐标函数
        GT911.touch_loc.display = GK_GetDisplay(CFG_TARGET_DISPLAY_NAME);
        result=Touch_InstallDevice(CFG_GT911_TOUCH_NAME,&GT911);//添加驱动到Touch
        if(!result)
            return false;

        result = true;
    }

    return result;
}

void touch_test(void)
{
    u8 temp[6];
    u8 i,touch_num = 0, Val_1 = 0;
    u16 xy;

    while(1)
    {
        do
        {
            GT911_RD_Reg(0x814E, &Val_1, 1);
            Djy_EventDelay(10000);
        }while(!(Val_1 & (1 << 7)));
//        GT911_RD_Reg(0x814f, &Val, 1);
        if(Val_1 & (1 << 7))
        {
            touch_num = Val_1&0x0f;
            printf("touch_num = %d \r\n",touch_num);
            if(touch_num == 0)
            {
                GT911_WR_Reg(0x814E,&touch_num,1);
                printf("没有触摸\r\n");
            }
            else
            {
//                for(i=0;i<touch_num;i++)
//                {
                   GT911_RD_Reg(0X814f,temp,7); //读取XY坐标值
                   printf("track id = %d \r\n",temp[0]);
                   xy = temp[5] | (temp[6] << 8);
                   printf("size = %d \r\n", xy);
                   xy = temp[1] | (temp[2] << 8);
                   printf("x=%d.\r\n",xy);
                   xy = temp[3] | (temp[4] << 8);
                   printf("y=%d.\r\n",xy);
//                }
            }
//            else
//            {
//                printf("touch_num = %d ",touch_num);
//            }
        }
//        if(GT911_RD_Reg(0x814f, &temp[0], 7) == 7)
//        {
//            printf("track id = %d \r\n",temp[0]);
//            xy = temp[5] | (temp[6] << 8);
//            printf("size = %d \r\n", xy);
//            xy = temp[1] | (temp[2] << 8);
//            printf("x=%d.\r\n",xy);
//            xy = temp[3] | (temp[4] << 8);
//            printf("y=%d.\r\n",xy);
////            Djy_EventDelay(1000*1000);
//        }
        touch_num = 0;
        GT911_WR_Reg(0x814E,&touch_num,1);
    }
}
