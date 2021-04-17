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
// 文件名     ：canbus.h
// 模块描述: CAN HAL抽象层，介于数据链路层和物理层之间。
// 模块版本: V1.00
// 创建人员: zhb
// 创建时间: 04/09.2018
// =============================================================================
#ifndef __CAN_BUS_H__
#define __CAN_BUS_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdio.h>

// CAN总线常用波特率.
typedef enum {
    EN_CAN_BUS_BAUD_5K = 5,                     //    5 Kbps (0)
    EN_CAN_BUS_BAUD_10K=10,                        //   10 Kbps (1)
    EN_CAN_BUS_BAUD_20K=20,                        //   20 Kbps (2)
    EN_CAN_BUS_BAUD_40K=40,                        //   40 Kbps (3)
    EN_CAN_BUS_BAUD_50K=50,                        //   50 Kbps (4)
    EN_CAN_BUS_BAUD_80K=80,                        //   80 Kbps (5)
    EN_CAN_BUS_BAUD_100K=100,                     //   100 Kbps (6)
    EN_CAN_BUS_BAUD_125K=125,                     //   125 Kbps (7)
    EN_CAN_BUS_BAUD_200K=200,                     //   200 Kbps (8)
    EN_CAN_BUS_BAUD_250K=250,                     //   250 Kbps (9)
    EN_CAN_BUS_BAUD_400K=400,                     //   400 Kbps (10)
    EN_CAN_BUS_BAUD_500K=500,                    //    500 Kbps (11)
    EN_CAN_BUS_BAUD_666K=666,                    //    660 Kbps (12)
    EN_CAN_BUS_BAUD_800K=800,                    //    800 Kbps (13)
    EN_CAN_BUS_BAUD_1000K=1000                   //  1000 Kbps (14)
}CanBaudRateDef;


//CAN控制操作码
typedef enum CAN_CTRL_CMD
{
    EN_CAN_BUS_RESET=0,                   //复位CAN控制器
    EN_CAN_BUS_SET_BAUDRATE,              //设置CAN通信波特率
    EN_CAN_BUS_GET_BAUDRATE,              //获取CAN通信波特率
    EN_CAN_BUS_SET_FILTER,                //设置CAN滤波属性
    EN_CAN_BUS_GET_FILTER,                //获取CAN滤波属性
}CanCtrlCmd;

//CAN发送模式
typedef enum CAN_SEND_MODE
{
    EN_CAN_BUS_SEND_ASYN_MODE=0,    //异步发送
    EN_CAN_BUS_SEND_SYN_MODE,       //同步发送
}CanSendMode;

//CAN报文类型
typedef enum
{
    EN_CAN_BUS_STD=(unsigned char)(0x00),
    EN_CAN_BUS_RTR=(unsigned char)(0x40),
    EN_CAN_BUS_EXD=(unsigned char)(0x80)
}CanFrameType;

//CAN总线错误类型
typedef enum
{
    EN_CAN_BUS_BUS_OFF_ERR=0,           //总线关闭
    EN_CAN_BUS_CRC_ERR,                 //CRC错误
    EN_CAN_BUS_BIT_ERR,                 //位错误,包括显性位错误和阴性位错误
    EN_CAN_BUS_STUFF_ERR,               //填充错误
    EN_CAN_BUS_FORMAT_ERR,              //帧格式错误
    EN_CAN_BUS_ACK_ERR,                 //确认错误
    EN_CAN_BUS_LOSS_ARB_ERR,            //仲裁失利
    EN_CAN_BUS_SEND_BAD,                //发送失败
    EN_CAN_BUS_RST_FAILED,              //复位失败
}CanErrType;


//CAN总线错误统计
typedef struct
{
    uint64_t BusOffErrCnt;    //总线关闭错误计数器
    uint64_t CrcErrCnt;       //CRC错误计数器
    uint64_t BitErrCnt;       //位错误计数器
    uint64_t StuErrCnt;       //填充错误计数器
    uint64_t FormatErrCnt;    //格式错误计数器
    uint64_t ACKErrCnt;       //ACK错误计数器
    uint64_t LossArbCnt;      //仲裁失利计数器
}CanErrStatDef;


//CANBUS数据统计
typedef struct
{
    uint64_t AppSndCnt;          //CANBUS上层发送CAN报文帧数
    uint64_t HardSndCnt;         //CAN控制器成功发送CAN报文帧数
    uint64_t HardRcvCnt;         //CAN控制器接收CAN报文帧数
    uint64_t AppRcvCnt;          //CANBUS上层接收CAN报文帧数
    uint64_t SendSempTimeoutCnt; //CANBUS发送报文时pend信号量超时
    uint64_t RecvSempTimeoutCnt; //CANBUS接收报文时pend信号量超时
    uint64_t LostSendFrameCnt;   //CANBUS由于发送缓冲区满丢失CAN报文帧数
    uint64_t LostRecvFrameCnt;   //CANBUS由于接收缓冲区满丢失CAN报文帧数
    uint64_t SndPkgBadCnt;       //CAN控制器发送失败次数，这里指CAN控制器未成功将报文发送到总线
    uint64_t RstFailedCnt;       //CAN控制器复位失败次数
    CanErrStatDef CanErrStat;    //CAN错误计数
}CanStatDef;


#pragma pack(1)
typedef struct CAN_Frame
{
    CanFrameType Type;  //CAN帧数据类型，CANBUS支持CAN2.0A及CAN2.0B，即支持标准帧，扩展帧及远程帧三类。
    uint8_t DLC;        //CAN帧有效数据字节数.
    uint32_t Id;        //CAN帧Id，如果是标准帧，则Id为11位；若是扩展帧则为29位。不论是标准帧还是扩展帧均按LSB格式。
    uint8_t Data[8];    //CAN帧的有效数据缓冲区，一帧CAN报文最大有效字节数为8.
}CanFarmeDef;
#pragma pack()

struct CANBusCB;
// =============================================================================
// 功能：CAN控制器将一帧CAN报文发送到CAN总线上。
// 参数：Frame,CAN帧结构体指针。
// 返回：成功发送则返回true，失败则返回false。
// =============================================================================
typedef bool_t  (*fnCanWrite)(struct CANBusCB * CANBus,CanFarmeDef *Frame);

// =============================================================================
// 功能：CAN控制器控制操作函数，用于设置CAN通信波特率，复位，滤波参数等功能。
// 参数：ctrlcmd,CAN控制操作码，具体参考enum CAN_CTRL_CMD;
//     param1,CAN控制操作参数1；
//     param2,CAN控制操作参数2;
// 返回：成功发送则返回true，失败则返回false。
// =============================================================================
typedef bool_t (*fnCanCtrl)(struct CANBusCB * CANBus,uint8_t ctrlcmd,ptu32_t param1,ptu32_t param2);

struct CanDev
{
   char *ChipName;           //芯片名称，如stm32系列片上三个CAN控制器可命名为“MCAN1/MCAN2/MCAN3”.
   CanBaudRateDef BaudRate;  //通信波特率
   fnCanWrite CanWrite;      //CAN控制器驱动中发送一帧函数
   fnCanCtrl  CanCtrl;       //CAN控制器控制函数，用于设置CAN通信波特率，CAN滤波等属性.
   ptu32_t PrivateTag;       //个性标记，由具体驱动解析
};


//CAN总线控制块结构体,本模块可见
struct CANBusCB
{
    struct Object          *CAN_BusNode;      //总线资源节点
    struct SemaphoreLCB    *CAN_BusSemp;     //CANBUS保护信号量,用于控制并发访问
    char                   *BusName;         //CANBUS名称，与CAN控制器名称保持一致
    CanBaudRateDef         BaudRate;         //通信波特率
    fnCanWrite             CanWrite;         //CANBUS发送帧钩子函数，底层CAN控制驱动注册
    fnCanCtrl              CanCtrl;          //CANBUS控制钩子函数，底层CAN控制驱动注册
    struct RingBuf         *RcvRing;         //CANBUS接收环形缓冲区
    CanStatDef             CanStat;          //CANBUS统计
    ptu32_t                PrivateTag;       //个性标记，由具体驱动解析
};


//函数声明
 // =============================================================================
 // 功能：将CAN总线类型结点挂接到DjyBus根结点。
 // 参数：para,无实际意义。
 // 返回：返回建立的资源结点指针，失败时返回NULL。
 // =============================================================================
struct Object * ModuleInstall_CANBus();
// =============================================================================
// 功能：添加CAN总线到CAN总线类型节点下，每个CAN控制器对应一条CAN总线。
// 参数：pNewCanDev,CAN Dev结构体指针。
// 返回：返回添加的CANBUS控制块结构体指针，失败时返回NULL。
// =============================================================================
struct CANBusCB *CAN_BusAdd(struct CanDev *pNewCanDev);
// =============================================================================
// 功能：添加CAN总线到CAN总线类型节点下，每个CAN控制器对应一条CAN总线，CAN总线控制块
//       由调用方提供。
// 参数：pNewCanDev,CAN Dev结构体指针；
//       NewCAN,CANBUS控制块数据结构体。
// 返回：返回添加的CANBUS控制块结构体指针，失败时返回NULL。
// =============================================================================
struct CANBusCB * CAN_BusAdd_s(struct CanDev *pNewCanDev,\
                                   struct CANBusCB *NewCAN);
// =============================================================================
// 功能：删除在CAN总线类型上的CANBUS指定结点。
// 参数：DelCAN,CANBUS控制块指针。
// 返回：true,删除成功;false,删除失败
// =============================================================================
bool_t CAN_BusDelete(struct CANBusCB *DelCAN);
// =============================================================================
// 功能：删除在CAN总线类型上的CANBUS指定结点。
// 参数：DelCAN,CANBUS控制块指针。
// 返回：true,删除成功;false,删除失败
// =============================================================================
bool_t CAN_BusDelete_s(struct CANBusCB *DelCAN);
// =============================================================================
// 功能：通过总线名称查找CANBUS句柄。
// 参数：BusName,CANBUS总线名称;
// 返回：若成功找到则返回CANBUS的句柄(一个无符号的32位数)，失败则返回NULL.
// =============================================================================
struct CANBusCB* CAN_BusFind(char *BusName);
// =============================================================================
// 功能：CANBUS写一帧CAN报文函数，通过该函数，可将需要发送的一帧CAN报文通过指定CANBUS(指定的CAN控制器)发送到CAN总线上。
// 参数：CANBus,CANBUS控制块结构体指针;
//     Frame,CAN帧结构体指针;
// 返回：发送成功则返回true，若发送失败，则返回false.
// =============================================================================
bool_t CAN_BusSendFrame(struct CANBusCB * CANBus,CanFarmeDef *Frame,CanSendMode Mode);
// =============================================================================
// 功能：CANBUS读函数，通过该函数，可读取指定CANBUS(指定CAN控制器)上接收的一帧CAN报文，底层CAN控制器接收到数据后会
//     存放在一个环形缓冲区中.
// 参数：CANBus,CANBUS控制块结构体指针;
//     Frame,CAN帧结构体指针;
// 返回：返回成功接收数据长度，若接收失败，则返回-1.
// =============================================================================
bool_t CAN_BusRecvFrame(struct CANBusCB * CANBus,CanFarmeDef *Frame);
// =============================================================================
// 功能：CANBUS控制函数，通过该函数可对指定CANBUS(指定CAN控制器)进行参数设置或其它操作，具体参见canbus.h中
//     CAN_CTRL_CMD定义。
// 参数：CANBus,CANBUS控制块结构体指针;
//     CtrlCmd,控制码，具体参见canbus.h中CAN_CTRL_CMD定义;
//     param,控制所需参数;
//     timeout，超时时间，指等待总线空闲的时间.
// 返回：成功操作则返回true,失败则返回false.
// =============================================================================
bool_t CAN_BusCtrl(struct CANBusCB * CANBus,uint8_t CtrlCmd,ptu32_t param1,ptu32_t param2);



bool_t CAN_BusAddRcvFrameToRing(struct CANBusCB * CANBus,CanFarmeDef *Frame);

#ifdef __cplusplus
}
#endif
#endif
