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

// 文件名     ：cpu_peri_can.c
// 模块描述: MCAN驱动
// 模块版本: V1.00
// 创建人员: zhb
// 创建时间: 05/01.2017
// =============================================================================
/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include <assert.h>
#include "cpu_peri.h"
#include "ring.h"
#include "os.h"
#include "stdlib.h"
#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"
#include "lock.h"
#include "msgqueue.h"
#include "board-config.h"
#include <shell.h>
#include <dbug.h>

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern bool_t CAN_Main(uint8_t baudrate);
//  CAN_Main(CFG_CPU_CAN_BAUDRATE);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip can"//CPU的can外设驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_CAN == false )
//#warning  " cpu_onchip_can  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_CAN    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,512,
#define CFG_CPU_CAN_BAUDRATE   125       //"CAN BaudRate",
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,

#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
//@#$%component end configure




/*---------------------------------------------------------------------------
 *      Definitions
 *---------------------------------------------------------------------------*/
#define CN_CAN1_BASE_ADDR         ((uint32_t)(0x40000000+0x6400))
#define CN_CAN3_BASE_ADDR         ((uint32_t)(0x40000000+0x3400))
#define CN_CAN1                               0
#define CN_CAN3                               1
#define CN_CAN_RX_BUF_SIZE                    2048
#define CN_CAN_DEV_MONITOR_TIME               2*1000*1000
#define CN_CAN_BUS_OFF_RECOVERY_TIME          1*1000*1000

#define CAN_SJW_1TQ      ((uint32_t)0x00000000U)     /*!< 1 time quantum */
#define CAN_SJW_2TQ      ((uint32_t)CAN_BTR_SJW_0)  /*!< 2 time quantum */
#define CAN_SJW_3TQ      ((uint32_t)CAN_BTR_SJW_1)  /*!< 3 time quantum */
#define CAN_SJW_4TQ      ((uint32_t)CAN_BTR_SJW)    /*!< 4 time quantum */
#define CAN_TS1_1TQ      ((uint32_t)0x00000000U)   /*!< 1 time quantum  */
#define CAN_TS1_2TQ      ((uint32_t)CAN_BTR_TS1_0)                                    /*!< 2 time quantum  */
#define CAN_TS1_3TQ      ((uint32_t)CAN_BTR_TS1_1)                                    /*!< 3 time quantum  */
#define CAN_TS1_4TQ      ((uint32_t)(CAN_BTR_TS1_1 | CAN_BTR_TS1_0))                  /*!< 4 time quantum  */
#define CAN_TS1_5TQ      ((uint32_t)CAN_BTR_TS1_2)                                    /*!< 5 time quantum  */
#define CAN_TS1_6TQ      ((uint32_t)(CAN_BTR_TS1_2 | CAN_BTR_TS1_0))                  /*!< 6 time quantum  */
#define CAN_TS1_7TQ      ((uint32_t)(CAN_BTR_TS1_2 | CAN_BTR_TS1_1))                  /*!< 7 time quantum  */
#define CAN_TS1_8TQ      ((uint32_t)(CAN_BTR_TS1_2 | CAN_BTR_TS1_1 | CAN_BTR_TS1_0))  /*!< 8 time quantum  */
#define CAN_TS1_9TQ      ((uint32_t)CAN_BTR_TS1_3)                                    /*!< 9 time quantum  */
#define CAN_TS1_10TQ     ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_0))                  /*!< 10 time quantum */
#define CAN_TS1_11TQ     ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_1))                  /*!< 11 time quantum */
#define CAN_TS1_12TQ     ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_1 | CAN_BTR_TS1_0))  /*!< 12 time quantum */
#define CAN_TS1_13TQ     ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_2))                  /*!< 13 time quantum */
#define CAN_TS1_14TQ     ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_2 | CAN_BTR_TS1_0))  /*!< 14 time quantum */
#define CAN_TS1_15TQ     ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_2 | CAN_BTR_TS1_1))  /*!< 15 time quantum */
#define CAN_TS1_16TQ     ((uint32_t)CAN_BTR_TS1) /*!< 16 time quantum */

#define CAN_TS2_1TQ      ((uint32_t)0x00000000U)                       /*!< 1 time quantum */
#define CAN_TS2_2TQ      ((uint32_t)CAN_BTR_TS2_0)                    /*!< 2 time quantum */
#define CAN_TS2_3TQ      ((uint32_t)CAN_BTR_TS2_1)                    /*!< 3 time quantum */
#define CAN_TS2_4TQ      ((uint32_t)(CAN_BTR_TS2_1 | CAN_BTR_TS2_0))  /*!< 4 time quantum */
#define CAN_TS2_5TQ      ((uint32_t)CAN_BTR_TS2_2)                    /*!< 5 time quantum */
#define CAN_TS2_6TQ      ((uint32_t)(CAN_BTR_TS2_2 | CAN_BTR_TS2_0))  /*!< 6 time quantum */
#define CAN_TS2_7TQ      ((uint32_t)(CAN_BTR_TS2_2 | CAN_BTR_TS2_1))  /*!< 7 time quantum */
#define CAN_TS2_8TQ      ((uint32_t)CAN_BTR_TS2)                      /*!< 8 time quantum */

#define CAN_BRP_27       (uint32_t)(0x1A&CAN_BTR_BRP)

#define CAN_ID_STD             ((uint32_t)0x00000000U)  /*!< Standard Id */
#define CAN_ID_EXT             ((uint32_t)0x00000004U)  /*!< Extended Id */

#define CAN_RTR_DATA           ((uint32_t)0x00000000U)  /*!< Data frame */
#define CAN_RTR_REMOTE         ((uint32_t)0x00000002U)  /*!< Remote frame */

#define CAN_FILTERSCALE_16BIT     ((uint8_t)0x00U)  /*!< Two 16-bit filters */
#define CAN_FILTERSCALE_32BIT     ((uint8_t)0x01U)  /*!< One 32-bit filter  */

#define CAN_FILTERMODE_IDMASK     ((uint8_t)0x00U)  /*!< Identifier mask mode */
#define CAN_FILTERMODE_IDLIST     ((uint8_t)0x01U)  /*!< Identifier list mode */

#define CAN_FILTER_FIFO0       ((uint8_t)0x00U)  /*!< Filter FIFO 0 assignment for filter x */
#define CAN_FILTER_FIFO1       ((uint8_t)0x01U)  /*!< Filter FIFO 1 assignment for filter x */
#define CN_PRINT_PREFIX  "   "


#define CN_CAN_MSGQ_LEN        14
#define CN_CAN_MSGQ_NUM       100

typedef struct {
    bool_t RcvFlag;
    struct RingBuf *RcvRing;
    uint32_t BaudRate;
}CAN_DevCtrl;

/**
  * @brief  CAN Rx message structure definition
  */
typedef struct
{
  uint32_t ExtId;       /*!< Specifies the extended identifier.
                             This parameter must be a number between Min_Data = 0 and Max_Data = 0x1FFFFFFF */
  uint32_t IDE;         /*!< Specifies the type of identifier for the message that will be received.
                              This parameter can be a value of @ref CAN_Identifier_Type */

  uint32_t RTR;         /*!< Specifies the type of frame for the received message.
                              This parameter can be a value of @ref CAN_remote_transmission_request */
  uint32_t DLC;         /*!< Specifies the length of the frame that will be received.
                             This parameter must be a number between Min_Data = 0 and Max_Data = 8 */

  uint8_t Data[8];      /*!< Contains the data to be received.
                             This parameter must be a number between Min_Data = 0 and Max_Data = 0xFF */

  uint32_t FMI;         /*!< Specifies the index of the filter the message stored in the mailbox passes through.
                             This parameter must be a number between Min_Data = 0 and Max_Data = 0xFF */

  uint32_t FIFONumber;  /*!< Specifies the receive FIFO number.
                             This parameter can be CAN_FIFO0 or CAN_FIFO1 */
}CanRxMsg;
/*---------------------------------------------------------------------------
 *      Internal variables
 *---------------------------------------------------------------------------*/
static uint32_t gs_CANBaudRate[CN_CAN_NUM];
static CAN_DevCtrl *CAN_DevCtrlPtr=NULL;
static tagCanFilterPara gs_ptagCanFilterPara[CN_CAN_NUM];
static u64 gs_u64AppSndCnt[CN_CAN_NUM];
static u64 gs_u64HardSndCnt[CN_CAN_NUM];
static u64 gs_u64HardRcvCnt[CN_CAN_NUM];
static u64 gs_u64AppRcvCnt[CN_CAN_NUM];
static u64 gs_u64BusOffCnt[CN_CAN_NUM];     //Bus off err
static u64 gs_u64PassiveErrCnt[CN_CAN_NUM]; //Error passive flag
static u64 gs_u64CrcErrCnt[CN_CAN_NUM];     //Crc err
static u64 gs_u64PasBitErrCnt[CN_CAN_NUM];  //隐性位错误
static u64 gs_u64DomBitErrCnt[CN_CAN_NUM];  //显性位错误
static u64 gs_u64FoErrCnt[CN_CAN_NUM];      //format err
static u64 gs_u64StErrCnt[CN_CAN_NUM];      //bit stuff err
static u64 gs_u64ACKErrCnt[CN_CAN_NUM];     //ACK err
static u64 gs_MonitorErrRstCnt[CN_CAN_NUM];
static u64 gs_u64RcvPkgBadCnt[CN_CAN_NUM];
static u64 gs_u64SndPkgBadCnt[CN_CAN_NUM];//数据包不完整，有效数据不为8个字节
static u64 gs_u64RstFailedCnt[CN_CAN_NUM];
static u64 gs_u64IntCnt[6];
static uint8_t CAN_MonitorStack[0x1000];
static uint8_t CAN_SndStack[0x1000];
static bool_t gs_CanDebugFlag=false;
static bool_t s_bBusOffFlag[CN_CAN_NUM];
static struct MsgQueue * gs_ptCanSndMsgQ;
bool_t bQiYongFlag[CN_CAN_NUM];

/*---------------------------------------------------------------------------
 * 函数声明
 *---------------------------------------------------------------------------*/
static void CAN_ISR_Handler(ptu32_t IntLine);
bool_t CAN_Hard_Init(uint8_t byCanNo,uint32_t baudrate,tagCanFilterPara \
        * pFilterConfPara);

/*******************************************************************************
功能:复位CAN控制器。
参数:无。
输出:无。
*********************************************************************************/
//static bool_t Sh_CAN_Reset(char *param)
bool_t canrst(char *param)
{
    char *word_ChipNum,*word_trail,*next_param;
    uint8_t byCanNo;
    bool_t ret;
    //提取1个参数
    word_ChipNum = shell_inputs(param,&next_param);
    word_trail = shell_inputs(next_param,&next_param);
    if((word_ChipNum == NULL)||(word_trail != NULL))
    {
      printf("\r\n格式错误，正确格式是：\r\n>d CAN控制器编号. \r\n");
      return false;
    }
    byCanNo = strtol(word_ChipNum, (char **)NULL, 0);
    if(byCanNo>=2)
    {
        printf("The Chip Num must be one of 0/1!\r\n");
        return false;
    }
    ret=CAN_Hard_Init(byCanNo,gs_CANBaudRate[byCanNo],&gs_ptagCanFilterPara[byCanNo]);
    if(!ret)
        printf("CAN rst failed.\r\n");
    else
        printf("CAN rst success.\r\n");
    return true;
}



/*******************************************************************************
功能:获取CAN总线通信统计数据
参数:无。
输出:无。
*********************************************************************************/
//static void Sh_CAN_Stat(char *param)
bool_t canstat(char *param)
{
   uint32_t data[2];//used to print the s64 type
   uint8_t i;
   uint64_t time;
   time=DJY_GetSysTime();
   printf("CAN Stat:\r\n");
   for(i=0;i<CN_CAN_NUM;i++)
   {
      printf("CAN %d:\r\n",i);
      memcpy(data,&time,sizeof(data));
      printf("%s Current Time         :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_u64AppSndCnt[i],sizeof(data));
      printf("%s APP Snd Cnt          :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_u64HardSndCnt[i],sizeof(data));
      printf("%s Hard Snd Cnt         :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_u64HardRcvCnt[i],sizeof(data));
      printf("%s Hard Rcv Cnt         :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_u64AppRcvCnt[i],sizeof(data));
      printf("%s APP Rcv Cnt          :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_u64SndPkgBadCnt[i],sizeof(data));
      printf("%s Snd Bad Cnt          :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_u64RcvPkgBadCnt[i],sizeof(data));
      printf("%s Rcv Bad Cnt          :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_MonitorErrRstCnt[i],sizeof(data));
      printf("%s Monitor Err Rst Cnt  :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_u64RstFailedCnt[i],sizeof(data));
      printf("%s Rst failed Cnt       :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_u64IntCnt[3*i],sizeof(data));
      printf("%s RX0 INT CNT          :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_u64IntCnt[3*i+1],sizeof(data));
      printf("%s RX1 INT CNT          :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_u64IntCnt[3*i+2],sizeof(data));
      printf("%s SCE INT              :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_u64BusOffCnt[i],sizeof(data));
      printf("%s Bus Off Cnt          :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_u64CrcErrCnt[i],sizeof(data));
      printf("%s CRC Err Cnt          :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_u64PasBitErrCnt[i],sizeof(data));
      printf("%s Passive Bit Err Cnt  :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_u64DomBitErrCnt[i],sizeof(data));
      printf("%s Dominant Bit Err Cnt :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_u64FoErrCnt[i],sizeof(data));
      printf("%s Format Err Cnt       :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_u64StErrCnt[i],sizeof(data));
      printf("%s Stuff Err Cnt        :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
      memcpy(data,&gs_u64ACKErrCnt[i],sizeof(data));
      printf("%s ACK Err Cnt          :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
   }
   printf("The End.\r\n");

   return true;
}

/*******************************************************************************
功能:获取CAN可读寄存器的值。
参数:无.
输出:无。
*********************************************************************************/
//static void Sh_Read_CAN_Reg(char *param)
bool_t canreg(char *param)
{
      vu32 Reg;
      uint8_t i;
      tagCanReg *pCan;
      printf("CAN Reg:\r\n");
      for(i=0;i<CN_CAN_NUM;i++)
      {
          if(i==0)
          {
              pCan=(tagCanReg *)CN_CAN1_BASE_ADDR;
          }
          else
          {
              pCan=(tagCanReg *)CN_CAN3_BASE_ADDR;
          }
          printf("CAN %d:\r\n",i);
          Reg=pCan->MCR;
          printf("%s MCR     :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
          Reg=pCan->MSR;
          printf("%s MSR     :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
          Reg=pCan->TSR;
          printf("%s TSR     :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
          Reg=pCan->RF0R;
          printf("%s RF0R    :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
          Reg=pCan->RF1R;
          printf("%s RF0R    :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
          Reg=pCan->IER;
          printf("%s IER     :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
          Reg=pCan->ESR;
          printf("%s ESR     :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
          Reg=pCan->BTR;
          printf("%s BTR     :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      }

      return true;
}

/*******************************************************************************
功能:CAN控制块内存申请初始化。
参数:空。
返回:true--初始化成功，false--初始化失败。
******************************************************************************/
//static bool_t Sh_CAN_SetFilter(char *param)
bool_t canf(char *param)
{
    char *word_ChipNum,*word_FilterIdHigh,*word_FilterIdLow,*word_FilterMaskIdHigh,*word_FilterMaskIdLow;
    char *word_FilterFIFOAssignment,*word_FilterNumber,*word_FilterMode;
    char *word_FilterScale,*word_FilterActivation,*word_BankNumber;
    char *word_trail,*next_param;
    tagCanFilterPara Para;
    uint8_t byChipNum;

    word_ChipNum=shell_inputs(param,&next_param);
    word_FilterIdHigh = shell_inputs(param,&next_param);
    word_FilterIdLow=shell_inputs(next_param,&next_param);
    word_FilterMaskIdHigh=shell_inputs(next_param,&next_param);
    word_FilterMaskIdLow=shell_inputs(next_param,&next_param);
    word_FilterFIFOAssignment=shell_inputs(next_param,&next_param);
    word_FilterNumber=shell_inputs(next_param,&next_param);
    word_FilterMode=shell_inputs(next_param,&next_param);
    word_FilterScale=shell_inputs(next_param,&next_param);
    word_FilterActivation=shell_inputs(next_param,&next_param);
    word_BankNumber=shell_inputs(next_param,&next_param);
    word_trail = shell_inputs(next_param,&next_param);

    if((word_ChipNum==NULL)||(word_FilterIdHigh == NULL)||(word_FilterIdLow==NULL)||\
        (word_FilterMaskIdHigh==NULL)||(word_FilterMaskIdLow==NULL)||(word_FilterFIFOAssignment==NULL)||\
        (word_FilterNumber==NULL)||(word_FilterMode==NULL)||(word_FilterScale==NULL)||\
        (word_FilterActivation==NULL)||(word_BankNumber==NULL)||(word_trail != NULL))
    {
      printf("\r\nCAN过滤器参数不对.\r\n");
      return false;
    }
    byChipNum=strtol(word_ChipNum, (char **)NULL, 0);
    Para.FilterIdHigh=strtol(word_FilterIdHigh, (char **)NULL, 0);
    Para.FilterIdLow=strtol(word_FilterIdLow, (char **)NULL, 0);
    Para.FilterMaskIdHigh=strtol(word_FilterMaskIdHigh, (char **)NULL, 0);
    Para.FilterMaskIdLow=strtol(word_FilterMaskIdLow, (char **)NULL, 0);
    Para.FilterFIFOAssignment=strtol(word_FilterFIFOAssignment, (char **)NULL, 0);
    Para.FilterNumber=strtol(word_FilterNumber, (char **)NULL, 0);
    Para.FilterMode=strtol(word_FilterMode, (char **)NULL, 0);
    Para.FilterScale=strtol(word_FilterScale, (char **)NULL, 0);
    Para.FilterActivation=strtol(word_FilterActivation, (char **)NULL, 0);
    Para.BankNumber=strtol(word_BankNumber, (char **)NULL, 0);
    memcpy(&gs_ptagCanFilterPara[byChipNum],&Para,sizeof(tagCanFilterPara));
    CAN_Hard_Init(byChipNum,gs_CANBaudRate[byChipNum],&gs_ptagCanFilterPara[byChipNum]);
    return true;
}
/*******************************************************************************
功能:CAN控制器设置波特率。
参数:空。
返回:true--设置成功，false--设置失败。
******************************************************************************/
//static void Sh_CAN_Set_BaudRate(char *param)
bool_t canb(char *param)
{
    char *word_ChipNum,*word_BaudRate,*word_trail,*next_param;
    uint8_t byCanNo;
    uint32_t BaudRate;
    //提取1个参数

    word_ChipNum = shell_inputs(param,&next_param);
    word_BaudRate = shell_inputs(next_param,&next_param);
    word_trail = shell_inputs(next_param,&next_param);
    if((word_ChipNum == NULL)||(word_BaudRate==NULL)||(word_trail != NULL))
    {
      printf("\r\n格式错误，正确格式是：\r\n>d CAN控制器编号+波特率值+Enter. \r\n");
      return false;
    }
    byCanNo = strtol(word_ChipNum, (char **)NULL, 0);
    if(byCanNo>=2)
    {
        printf("The Chip Num must be one of 0/1!\r\n");
        return false ;
    }
    BaudRate = strtol(word_BaudRate, (char **)NULL, 0);
    if((BaudRate!=125)||(BaudRate!=250))
    {
        printf("The baudrate must be 125 or 250.\r\n");
        return false;
    }
    gs_CANBaudRate[byCanNo]=BaudRate;
    CAN_Hard_Init(byCanNo,BaudRate,&gs_ptagCanFilterPara[byCanNo]);
    return true;
}

/*******************************************************************************
功能:置位CAN调试标识符。
参数:空。
返回:无。
******************************************************************************/
//static void Sh_CAN_Set_DebugFlag(char *param)
bool_t cands(char *param)
{
    gs_CanDebugFlag=true;
    return true;
}

/*******************************************************************************
功能:清CAN调试标识符。
参数:空。
返回:无。
******************************************************************************/
//static void Sh_CAN_Clear_DebugFlag(char *param)
bool_t candc(char *param)
{
    gs_CanDebugFlag=false;
    return true;
}
/*******************************************************************************
功能:CAN控制块内存申请初始化。
参数:空。
返回:true--初始化成功，false--初始化失败。
******************************************************************************/
bool_t __CAN_MemInit()
{
    CAN_DevCtrl *CAN_DevCtrlTempptr=NULL;
    uint8_t i;
    struct RingBuf *RingBuf;
    CAN_DevCtrlPtr=(CAN_DevCtrl *)malloc(sizeof(CAN_DevCtrl)*CN_CAN_NUM);
    if(!CAN_DevCtrlPtr)
        return false;
    for(i=0;i<CN_CAN_NUM;i++)
    {
       CAN_DevCtrlTempptr=CAN_DevCtrlPtr+i;
       RingBuf=(struct RingBuf *)malloc(sizeof(struct RingBuf));
       if(!RingBuf)
       {
          goto ERRORDeal;
       }
       RingBuf->buf=(uint8_t *)malloc(CN_CAN_RX_BUF_SIZE);
       if(!RingBuf->buf)
       {
          goto ERRORDeal;
       }

        CAN_DevCtrlTempptr->RcvRing=RingBuf;
    }
    return true;

ERRORDeal:
    for(i=0;i<CN_CAN_NUM;i++)
    {
        CAN_DevCtrlTempptr=CAN_DevCtrlPtr+i;
        if(CAN_DevCtrlTempptr->RcvRing)
        {
            free(CAN_DevCtrlTempptr->RcvRing);
        }
    }
    free(CAN_DevCtrlPtr);
    return false;
}
//----CAN滤波器设置------------------------------------------------------------
//功能:设置CAN控制器 滤波功能，可通过设置滤波参数，让CAN控制器只接收指定ID的CAN报文，其他报文直接丢弃.
//参数: byChip,CAN控制器编号;
//     pFilterConfPara:CAN滤波器配置参数
//返回: 成功返回true,否则返回false.
//-----------------------------------------------------------------------------
bool_t CAN_SetFilter(uint8_t byChip,tagCanFilterPara *pFilterConfPara)
{
    bool_t ret;
    if(byChip>=2)
    {
        printf("FUNCTION %s para is invalid.\r\n",__FUNCTION__);
        return false;
    }
    if(pFilterConfPara==NULL)
    {
        printf("FUNCTION %s para is invalid.\r\n",__FUNCTION__);
        return false;
    }

    memcpy(&gs_ptagCanFilterPara[byChip],pFilterConfPara,sizeof(tagCanFilterPara));
    ret=CAN_Hard_Init(byChip,gs_CANBaudRate[byChip], pFilterConfPara);
    return ret;
}
//----CAN波特率设置------------------------------------------------------------
//功能: 设置CAN通信波特率
//参数: byChip,CAN控制器编号;
//     baudrate,波特率，只支持125k和250k两种.
//返回: 成功则返回true,否则返回false.
//-----------------------------------------------------------------------------
bool_t CAN_SetBaudRate(uint8_t byChip,uint32_t baudrate)
{
    bool_t ret;
    if(byChip>=2)
    {
        printf("FUNCTION %s para is invalid.\r\n",__FUNCTION__);
        return false;
    }
    gs_CANBaudRate[byChip]=baudrate;
    ret=CAN_Hard_Init(byChip,baudrate, &gs_ptagCanFilterPara[byChip]);
    return ret;
}

//----CAN滤波器设置------------------------------------------------------------
//功能: 初始化CAN中断配置
//参数: pCanPara,CAN控制器配置参数结构体指针
//返回: 成功初始化则返回true,否则则返回false.
//-----------------------------------------------------------------------------
 bool_t __SetFilter(uint8_t byCanNo,tagCanFilterPara* pFilterConfig)
{
    uint32_t filternbrbitpos = 0;
    tagCanReg *pCan;
    if(byCanNo==0)
    {
      pCan=(tagCanReg *)CN_CAN1_BASE_ADDR;
    }
    else if(byCanNo==1)
    {
      pCan=(tagCanReg *)CN_CAN3_BASE_ADDR;
    }
    else
    {
        printf("FUNCTION %s para is invalid.\r\n",__FUNCTION__);
        return false;
    }

    memcpy(&gs_ptagCanFilterPara[byCanNo],pFilterConfig,sizeof(tagCanFilterPara));

    filternbrbitpos = ((uint32_t)1) << pFilterConfig->FilterNumber;
    //Init mode for the filter
    pCan->FMR|=(uint32_t)CAN_FMR_FINIT;
    //Select the start slave bank
    pCan->FMR&=~(uint32_t)CAN_FMR_CAN2SB;
    pCan->FMR|=(uint32_t)(pFilterConfig->BankNumber<<8);
    //Filter Deactivation
    pCan->FA1R&=~(uint32_t)filternbrbitpos;
    //Set filter scale
    if(pFilterConfig->FilterScale==CAN_FILTERSCALE_16BIT)
    {
        //16 bit scale for the filter
        pCan->FS1R&=~(uint32_t)filternbrbitpos;
        //First 16 bit identifier and First 16 bit mask (mask mode)
        //Or First 16 bit identifier and Second 16 bit identifier(list mode)
        pCan->sFilterRegister[pFilterConfig->FilterNumber].FR1=
        ((0x0000FFFF & (uint32_t)pFilterConfig->FilterMaskIdLow) << 16) |
        (0x0000FFFF & (uint32_t)pFilterConfig->FilterIdLow);
        /* Second 16-bit identifier and Second 16-bit mask */
            /* Or Third 16-bit identifier and Fourth 16-bit identifier */
        pCan->sFilterRegister[pFilterConfig->FilterNumber].FR2 =
         ((0x0000FFFF & (uint32_t)pFilterConfig->FilterMaskIdHigh) << 16) |
          (0x0000FFFF & (uint32_t)pFilterConfig->FilterIdHigh);
    }
    else
    {
        /* 32-bit scale for the filter */
        pCan->FS1R |= filternbrbitpos;
        /* 32-bit identifier or First 32-bit identifier */
        pCan->sFilterRegister[pFilterConfig->FilterNumber].FR1 =
         ((0x0000FFFF & (uint32_t)pFilterConfig->FilterIdHigh) << 16) |
          (0x0000FFFF & (uint32_t)pFilterConfig->FilterIdLow);
        /* 32-bit mask or Second 32-bit identifier */
        pCan->sFilterRegister[pFilterConfig->FilterNumber].FR2 =
         ((0x0000FFFF & (uint32_t)pFilterConfig->FilterMaskIdHigh) << 16) |
          (0x0000FFFF & (uint32_t)pFilterConfig->FilterMaskIdLow);
    }
    //Filter mode
    if(pFilterConfig->FilterMode==CAN_FILTERMODE_IDMASK)
    {
        pCan->FM1R&=~(uint32_t)filternbrbitpos;
    }
    else
    {
        pCan->FM1R |= (uint32_t)filternbrbitpos;
    }
    //Filter FIFO ASSIGNMENT
    if (pFilterConfig->FilterFIFOAssignment == CAN_FILTER_FIFO0)
    {
       /* FIFO 0 assignation for the filter */
        pCan->FFA1R &= ~(uint32_t)filternbrbitpos;
    }
    if (pFilterConfig->FilterFIFOAssignment == CAN_FILTER_FIFO1)
    {
       /* FIFO 1 assignation for the filter */
        pCan->FFA1R |= (uint32_t)filternbrbitpos;
    }
    //Filter activation
    if(pFilterConfig->FilterActivation==ENABLE)
    {
        pCan->FA1R |= filternbrbitpos;
    }
    //Leave the init mode for the filter
    pCan->FMR&=~(uint32_t)CAN_FMR_FINIT;
    return true;
}
//----初始化CAN中断配置---------------------------------------------------------
//功能: 初始化CAN中断配置
//参数: pCanPara,CAN控制器配置参数结构体指针
//返回: 成功初始化则返回true,否则则返回false.
//-----------------------------------------------------------------------------

static void  __CAN_IntInit(uint8_t byCanNo)
{
    //挂中断线
    if(byCanNo==0)
    {
        //发送中断线
        Int_Register(CN_INT_LINE_CAN1_RX0);
        Int_IsrConnect(CN_INT_LINE_CAN1_RX0,(uint32_t(*)(ptu32_t))CAN_ISR_Handler);
        Int_SettoAsynSignal(CN_INT_LINE_CAN1_RX0);
        Int_ClearLine(CN_INT_LINE_CAN1_RX0);
        Int_RestoreAsynLine(CN_INT_LINE_CAN1_RX0);
        //发送中断线
        Int_Register(CN_INT_LINE_CAN1_RX1);
        Int_IsrConnect(CN_INT_LINE_CAN1_RX1,(uint32_t(*)(ptu32_t))CAN_ISR_Handler);
        Int_SettoAsynSignal(CN_INT_LINE_CAN1_RX1);
        Int_ClearLine(CN_INT_LINE_CAN1_RX1);
        Int_RestoreAsynLine(CN_INT_LINE_CAN1_RX1);
        //发送中断线
        Int_Register(CN_INT_LINE_CAN1_SCE);
        Int_IsrConnect(CN_INT_LINE_CAN1_SCE,(uint32_t(*)(ptu32_t))CAN_ISR_Handler);
        Int_SettoAsynSignal(CN_INT_LINE_CAN1_SCE);
        Int_ClearLine(CN_INT_LINE_CAN1_SCE);
        Int_RestoreAsynLine(CN_INT_LINE_CAN1_SCE);
    }
    else
    {
        //发送中断线
        Int_Register(CN_INT_LINE_CAN3_RX0);
        Int_IsrConnect(CN_INT_LINE_CAN3_RX0,(uint32_t(*)(ptu32_t))CAN_ISR_Handler);
        Int_SettoAsynSignal(CN_INT_LINE_CAN3_RX0);
        Int_ClearLine(CN_INT_LINE_CAN3_RX0);
        Int_RestoreAsynLine(CN_INT_LINE_CAN3_RX0);
        //发送中断线
        Int_Register(CN_INT_LINE_CAN3_RX1);
        Int_IsrConnect(CN_INT_LINE_CAN3_RX1,(uint32_t(*)(ptu32_t))CAN_ISR_Handler);
        Int_SettoAsynSignal(CN_INT_LINE_CAN3_RX1);
        Int_ClearLine(CN_INT_LINE_CAN3_RX1);
        Int_RestoreAsynLine(CN_INT_LINE_CAN3_RX1);
        //发送中断线
        Int_Register(CN_INT_LINE_CAN3_SCE);
        Int_IsrConnect(CN_INT_LINE_CAN3_SCE,(uint32_t(*)(ptu32_t))CAN_ISR_Handler);
        Int_SettoAsynSignal(CN_INT_LINE_CAN3_SCE);
        Int_ClearLine(CN_INT_LINE_CAN3_SCE);
        Int_RestoreAsynLine(CN_INT_LINE_CAN3_SCE);
    }

}

//----配置CAN通信波特率---------------------------------------------------------
//功能: 配置CAN通信波特率。
//参数: pCan,CAN控制器寄存器结构体指针；
//      baudrate,CAN通信波特率。
//返回: 成功返回true,失败返回false.
//说明：在充电桩应用中仅存在250k及125k的通信波特率，目前仅支持这两种波特率.CAN1/CAN2
//      时钟源均来自APB1,在sysinit.c中已将APB1的时钟设置为54MHz.
//      can_bd=(54M)/((BRP+1)*(1+(TS1+1)+(TS2+1))
//-----------------------------------------------------------------------------
 bool_t __SetBaudRate(uint8_t byCanNo,uint32_t baudrate)
{
    tagCanReg * pCan;
    if(byCanNo==0)
    {
      pCan=(tagCanReg *)CN_CAN1_BASE_ADDR;
    }
    else if(byCanNo==1)
    {
      pCan=(tagCanReg *)CN_CAN3_BASE_ADDR;
    }
    else
    {
        printf("FUNCTION %s para is invalid.\r\n",__FUNCTION__);
        return false;
    }

    if(baudrate==125)
    {
      pCan->BTR=(uint32_t)CAN_BRP_27|CAN_SJW_1TQ|CAN_TS1_13TQ|CAN_TS2_2TQ;
    }
    else if(baudrate==250)
    {
        pCan->BTR=(uint32_t)CAN_BRP_27|CAN_SJW_1TQ|CAN_TS1_5TQ|CAN_TS2_2TQ;
    }
    else
    {
        printf("FUNCTION %s para is invalid.\r\n",__FUNCTION__);
        return false;
    }
    gs_CANBaudRate[byCanNo]=baudrate;
    return true;

}


//extern void CAN_HalInit(void);
/*******************************************************************************
功能:CAN设备的初始化函数，初始化中断控制器，申请内存，初始化器件。
参数:byIndex--CAN控制器编号，0~1.
输出:true--初始化成功，false-初始化失败。
说明：
*********************************************************************************/
bool_t CAN_Hard_Init(uint8_t byCanNo,uint32_t baudrate,tagCanFilterPara \
        * pFilterConfPara)
{
    tagCanReg *pCan;
    static uint8_t flag=0;
    uint32_t i=0;
    CAN_DevCtrl *CAN_DevCtrlTempptr=NULL;
    struct RingBuf *rxRingBuf=NULL;
    bool_t ret=true;
    if(byCanNo==0)
    {
           pCan=(tagCanReg *)CN_CAN1_BASE_ADDR;
    }
    else if(byCanNo==1)
    {
        pCan=(tagCanReg *)CN_CAN3_BASE_ADDR;
    }
    else
    {
        printf("FUNCTION %s para is invalid.\r\n",__FUNCTION__);
        return false;
    }
    if(!flag)
    {
        if(__CAN_MemInit()==false)
        {
            printf("CAN Dev Control Block malloc failed.\r\n");
            return false;
        }
        //初始化接收环形缓冲区
        for(i=0;i<CN_CAN_NUM;i++)
        {
            CAN_DevCtrlTempptr=CAN_DevCtrlPtr+i;
            rxRingBuf=CAN_DevCtrlTempptr->RcvRing;
            Ring_Init(rxRingBuf,rxRingBuf->buf,CN_CAN_RX_BUF_SIZE);
        }
        flag++;
    }

    CAN_DevCtrlTempptr=CAN_DevCtrlPtr+byCanNo;
    CAN_DevCtrlTempptr->BaudRate=baudrate;
    CAN_DevCtrlTempptr->RcvFlag=false;
    //CAN_HalInit();
    //Exit from sleep mode
    pCan->MCR&=(~(uint32_t)CAN_MCR_SLEEP);
    //Request init
    pCan->MCR|=CAN_MCR_INRQ;
    //Wait the acknowledge
    while((pCan->MSR&CAN_MSR_INAK)!=CAN_MSR_INAK)
    {
        i++;
        if(i>10000000)
        {
            if(gs_CanDebugFlag)
            {
               printf("CAN %d init failed.\r\n",byCanNo);
            }
            gs_u64RstFailedCnt[byCanNo]++;
            return false;
        }
    }
    //Disable time triggered communication mode
    pCan->MCR&=~(uint32_t)CAN_MCR_TTCM;
    //Set automatic bus-off management
//  pCan->MCR|=CAN_MCR_ABOM;
    pCan->MCR&=~(uint32_t)CAN_MCR_ABOM;
    //Set automatic wake-up mode
//  pCan->MCR|=CAN_MCR_AWUM;
    pCan->MCR&=~(uint32_t)CAN_MCR_AWUM;
    //Set no automatic retransmission
    pCan->MCR&=~(uint32_t)CAN_MCR_NART;
    //Set transmit FIFO priority (first in first out)
    pCan->MCR|=CAN_MCR_TXFP;
    //Set recv FIFO locked mode
    pCan->MCR&=~(uint32_t)CAN_MCR_RFLM;

    //Set the bit timing register  APB1上 54MHz
    __SetBaudRate(byCanNo,baudrate);

    //Set filter configuration
    if(pFilterConfPara!=NULL)
    {
       __SetFilter(byCanNo,pFilterConfPara);
    }

     //Init int
    __CAN_IntInit(byCanNo);
    //Enabel Recv Fifo(FMPIE/FFIE/FOVIE) EPVIE BOFIE LECIE ERRIE interrupt
    pCan->IER|=CAN_IER_FMPIE0|CAN_IER_FFIE0|CAN_IER_FOVIE0|CAN_IER_FMPIE1\
        |CAN_IER_FFIE1|CAN_IER_FOVIE1|CAN_IER_BOFIE|CAN_IER_LECIE|CAN_IER_ERRIE;
    //Request leave init
    pCan->MCR&=~(uint32_t)CAN_MCR_INRQ;
    //Wait the acknowledge
    i=0;
    while((pCan->MSR&CAN_MSR_INAK)!=CAN_MSR_INAK)
    {
        i++;
        if(i>10000000)
        {
            if(gs_CanDebugFlag)
            {
               printf("CAN %d init failed.\r\n",byCanNo);
            }
            gs_u64RstFailedCnt[byCanNo]++;
            return false;

        }
    }
    ret=true;
    if(gs_CanDebugFlag)
    {
        printf("CAN %d init success.\r\n",byCanNo);
    }
    return ret;

}

//----选择一个空闲的发送邮箱-------------------------------------
//功能: 从编号0开始依次查询三个发送邮箱的状态是否为空，直到找到找到一个空邮箱，如果
//      3个发送邮箱均不为空，则返回-1.
//参数: byChip:  CAN控制器编号
//返回: 如果3个发送邮箱均不为空，则返回-1.其他则相应的返回0、1、2.
//-----------------------------------------------------------------------------
static sint8_t __SelectOneEmptyTxMalibox(uint8_t byChip)
{
    tagCanReg *pCan;
    if(byChip==0)
        pCan=(tagCanReg *)CN_CAN1_BASE_ADDR;
    else if(byChip==1)
        pCan=(tagCanReg *)CN_CAN3_BASE_ADDR;
    else
        return false;
    if((pCan->TSR&CAN_TSR_TME0)==CAN_TSR_TME0)
        return 0;
    else if((pCan->TSR&CAN_TSR_TME1)==CAN_TSR_TME1)
        return 1;
    else if((pCan->TSR&CAN_TSR_TME2)==CAN_TSR_TME2)
        return 2;
    else
        return -1;

}

//----检查某个指定发送邮箱是否成功将数据发送出去-------------------------------------
//功能: 检查某个指定发送邮箱是否成功将数据发送出去。
//参数: TxMailboxNo:  发送邮箱编号，0~2.
//返回: 如果发送成功返回true,失败则返回false.
//-----------------------------------------------------------------------------
static bool_t __IsCanTxOk(uint8_t byChip,uint8_t TxMailboxNo)
{
    tagCanReg *pCan;
    uint32_t state=0;
    if(byChip==0)
    {
      pCan=(tagCanReg *)CN_CAN1_BASE_ADDR;
    }
    else if(byChip==1)
    {
      pCan=(tagCanReg *)CN_CAN3_BASE_ADDR;
    }
    else
        return false;

    if(TxMailboxNo==0)
    {
        state=pCan->TSR&(CAN_TSR_RQCP0|CAN_TSR_TXOK0|CAN_TSR_TME0);
        if(gs_CanDebugFlag)
        {
            printf("Tx state:0x%08x.\r\n",state);
        }
        if(state==(CAN_TSR_RQCP0|CAN_TSR_TXOK0|CAN_TSR_TME0))
        {
            return true;
        }
    }
    else if(TxMailboxNo==1)
    {

        state=pCan->TSR&(CAN_TSR_RQCP1|CAN_TSR_TXOK1|CAN_TSR_TME1);
        if(gs_CanDebugFlag)
        {
            printf("Tx state:0x%08x.\r\n",state);
        }
        if(state==(CAN_TSR_RQCP1|CAN_TSR_TXOK1|CAN_TSR_TME1))
        {
            return true;
        }
    }
    else if(TxMailboxNo==2)
    {
        state=pCan->TSR&(CAN_TSR_RQCP2|CAN_TSR_TXOK2|CAN_TSR_TME2);
        if(gs_CanDebugFlag)
        {
            printf("Tx state:0x%08x.\r\n",state);
        }
        if(state==(CAN_TSR_RQCP2|CAN_TSR_TXOK2|CAN_TSR_TME2))
        {
            return true;
        }
    }
    else
        return false;
    return false;
}
//----CAN发送函数-------------------------------------
//功能: CAN发送函数
//参数:
       // byChip:  CAN控制器编号   0~1(为兼容项目组代码，保留此参数，在SR5333中该参数
//        为0，SR5333 V1.01版本中只使用了一个CAN控制器)
       // txBuf:   报文的发送缓冲区
       // len:     发送报文的长度,13的整数倍.
//返回: 成功发送的长度
//-----------------------------------------------------------------------------
uint32_t CAN_WriteData(uint8_t byChip, uint8_t* txBuf, uint32_t len)
{

    uint8_t pkgnum,i,j;
    bool_t ret;
    uint8_t buf[14];
    //参数检查
    if(txBuf==NULL)
    {
         if(gs_CanDebugFlag)
         {
             printf("Function:%s para invailed.\r\n",__FUNCTION__);
         }
         return 0;
    }
    if(byChip>2)
    {
       if(gs_CanDebugFlag)
       {
           printf("Function:%s para invailed.\r\n",__FUNCTION__);
       }
       return 0;

    }
    if((len%13!=0)||(len==0))
    {
        if(gs_CanDebugFlag)
        {
               printf("Function:%s para invailed.\r\n",__FUNCTION__);
        }
        return 0;
    }

     pkgnum=len/13;
     gs_u64AppSndCnt[byChip]+=pkgnum;
     buf[0]=byChip;
     for(i=0;i<pkgnum;i++)
     {
        for(j=0;j<13;j++)
         {
                buf[1+j]=txBuf[13*i+j];
         }
         ret=MsgQ_Send(gs_ptCanSndMsgQ,buf,CN_CAN_MSGQ_LEN,0, CN_MSGQ_PRIO_NORMAL);
         if(!ret)
         {
            if(gs_CanDebugFlag)
            {
                printf("CAN Snd MsgQ is full.\r\n");
            }
         }
    }

   return len;

}


//----底层CAN异步发送一帧CAN报文-------------------------------------
//功能: CAN发送函数
//参数:
       // byChip:  CAN控制器编号   0~1(为兼容项目组代码，保留此参数，在SR5333中该参数
//        为0，SR5333 V1.01版本中只使用了一个CAN控制器)
       // txBuf:   报文的发送缓冲区
//返回: 成功发送返回true,失败则返回false.
//-----------------------------------------------------------------------------
bool_t __CAN_HardTxOneFrame(uint8_t byChip, uint8_t* txBuf)
{
     tagCanReg *pCan;
     sint8_t TxMailboxNo;
     uint32_t Id=0;
     bool_t ret=false;

     if(byChip==0)
    {
      pCan=(tagCanReg *)CN_CAN1_BASE_ADDR;
    }
    else if(byChip==1)
    {
      pCan=(tagCanReg *)CN_CAN3_BASE_ADDR;
    }
     else
        return false;

       //check Tx mailbox is or not empty
      TxMailboxNo=__SelectOneEmptyTxMalibox(byChip);
    if(TxMailboxNo==-1)
    {
          if(gs_CanDebugFlag)
          {
               printf("No empty Tx MailBox.\r\n");
          }
          gs_u64SndPkgBadCnt[byChip]++;
          return false;

      }
    Id=0x0000;
    Id|=(uint32_t)txBuf[1]<<24;
    Id|=(uint32_t)txBuf[2]<<16;
    Id|=(uint32_t)txBuf[3]<<8;
    Id|=(uint32_t)txBuf[4];
       //Request the tx mailbox
     pCan->sTxMailBox[TxMailboxNo].TIR&=CAN_TI0R_TXRQ;
        //Set ID and RTR/IDE filed  Data frame Extend Frame
     pCan->sTxMailBox[TxMailboxNo].TIR|=(Id<<3)|(1<<2);
        //Set DLC filed
     pCan->sTxMailBox[TxMailboxNo].TDTR&=(uint32_t)0xFFFFFFF0U;
     pCan->sTxMailBox[TxMailboxNo].TDTR|=0x00000008U;
     //Set data filed
     pCan->sTxMailBox[TxMailboxNo].TDLR=(uint32_t)(txBuf[8]<<24)|\
                (uint32_t)(txBuf[7]<<16)|(uint32_t)(txBuf[6]<<8)|\
                (uint32_t)txBuf[5];

     pCan->sTxMailBox[TxMailboxNo].TDHR=(uint32_t)(txBuf[12]<<24)|\
                (uint32_t)(txBuf[11]<<16)|(uint32_t)(txBuf[10]<<8)\
                |(uint32_t)txBuf[9];
      //Request transmission
      pCan->sTxMailBox[TxMailboxNo].TIR|=CAN_TI0R_TXRQ;

    //Wait for 10ms
    DJY_EventDelay(5*mS);
    //查询是否成功发送出去
    if(__IsCanTxOk(byChip,TxMailboxNo))
    {
        gs_u64HardSndCnt[byChip]++;
        ret=true;
    }
    else
    {
        gs_u64SndPkgBadCnt[byChip]++;
           ret=false;
    }
    return ret;
}

//----CAN接收函数-------------------------------------
//功能: CAN发送函数
//参数:
      // byChip:  CAN控制器选择   0~4(为兼容项目组代码，保留此参数，在SR5333中该参数
      //  为0，SR5333 V1.01版本中只使用了一个CAN控制器)
      // txBuf:    报文的接收缓冲区
      // len:     请求读取的长度
      // pRd:     缓冲区读指针
//返回: 成功读取数据的长度
//-----------------------------------------------------------------------------
uint32_t CAN_ReadData(uint8_t byChip, uint8_t* rxBuf, uint32_t len, uint32_t *pRd)
{
    uint32_t rdLen=0x0000,ringlen,pkgnum,r_len;
    CAN_DevCtrl *CAN_DevCtrlTempptr=NULL;
    struct RingBuf *rxRingBuf=NULL;
    atom_low_t atom;
    //先作参数检查
    if(rxBuf==NULL)
         return 0;
    if(byChip>=2)
        return 0;
    CAN_DevCtrlTempptr=CAN_DevCtrlPtr+byChip;
    rxRingBuf=CAN_DevCtrlTempptr->RcvRing;
    atom = Int_LowAtomStart();
    ringlen=Ring_Check(rxRingBuf);
    Int_LowAtomEnd(atom);
    if(len>ringlen)
    {
        r_len=ringlen;
    }
    else
    {
        r_len=len;
    }
    rdLen=Ring_Read(rxRingBuf,rxBuf,r_len);
    pkgnum=rdLen/13;
    gs_u64AppRcvCnt[byChip]+=pkgnum;
    return rdLen;
}

//----将CAN硬件接收到的数据写进环形缓冲区中---------------------------------------------------------
//功能: 将CAN硬件接收到的数据写进环形缓冲区中，每帧向缓冲区写入13bytes.
//参数: rxMsg,CAN接收数据结构体;
//返回: 空
//-----------------------------------------------------------------------------
static void __WriteMsgToRing(uint8_t byChip,CanRxMsg rxMsg)
{
    uint8_t i;
    uint8_t buf[13];
    uint32_t can_id,tmp;
    CAN_DevCtrl *CAN_DevCtrlTempptr=NULL;
    struct RingBuf *rxRingBuf=NULL;
    buf[0]=0x88;
    can_id=rxMsg.ExtId;
    tmp=can_id;
    buf[1]=tmp;
    buf[2]=tmp>>8;
    buf[3]=tmp>>16;
    buf[4]=tmp>>24;
    for(i=0;i<8;i++)
    {
        buf[5+i]=rxMsg.Data[i];
    }
    CAN_DevCtrlTempptr=CAN_DevCtrlPtr+byChip;
    rxRingBuf=CAN_DevCtrlTempptr->RcvRing;
    Ring_Write(rxRingBuf,buf,13);
    gs_u64HardRcvCnt[byChip]++;

}
//----接收中断响应函数---------------------------------------------------------
//功能: 依次查询专用接收Buffer(Dedicated RX Buffer)，如果有数据则
//参数: byChip,CAN控制器编号，0、1;
//      byFiFoNo,接收FIFO编号，0、1.
//返回: 空
//-----------------------------------------------------------------------------
static void __CAN_Recv_FiFo_Handle(uint8_t byChip,uint8_t byFiFoNo)
{
    tagCanReg *pCan;
    CanRxMsg RcvMsg;
    uint8_t cnt,i,j,ret;
    if(byChip==0)
    {
      pCan=(tagCanReg *)CN_CAN1_BASE_ADDR;
    }
    else if(byChip==1)
    {
      pCan=(tagCanReg *)CN_CAN3_BASE_ADDR;
    }
    else
        return ;
    if(byFiFoNo==0)
        cnt=pCan->RF0R&(uint32_t)0x03;  //check if fifo0 msg pending
    else
        cnt=pCan->RF1R&(uint32_t)0x03;
    if(cnt==0)
        return;
    for(i=0;i<cnt;i++)
    {
        if(byFiFoNo==0)
            ret=pCan->RF0R&CAN_RF0R_FMP0;
        else
            ret=pCan->RF1R&CAN_RF0R_FMP0;
        if(ret)
        {
            RcvMsg.IDE=(uint8_t)0x04&(pCan->sFIFOMailBox[byFiFoNo].RIR);
            RcvMsg.RTR=(uint8_t)0x02&(pCan->sFIFOMailBox[byFiFoNo].RIR);
            if((RcvMsg.IDE==CAN_ID_STD)||(RcvMsg.RTR==CAN_RTR_REMOTE))
            {
                //release the fifo
                pCan->RF0R|=CAN_RF0R_RFOM0;
                continue;
            }
            else
            {
                RcvMsg.ExtId=(uint32_t)0x1FFFFFFF&(pCan->sFIFOMailBox[byFiFoNo].RIR>>3);
                RcvMsg.DLC=(uint8_t)0x0F&(pCan->sFIFOMailBox[byFiFoNo].RDTR);
                //Get the FMI
                RcvMsg.FMI=(uint8_t)0xFF&(pCan->sFIFOMailBox[byFiFoNo].RDTR>>8);
                //Get the data
                for(j=0;j<4;j++)
                {
                    RcvMsg.Data[j]=(uint8_t)0xFF&(pCan->sFIFOMailBox[byFiFoNo].RDLR>>(8*j));
                }
                for(j=0;j<4;j++)
                {
                    RcvMsg.Data[4+j]=(uint8_t)0xFF&(pCan->sFIFOMailBox[byFiFoNo].RDHR>>(8*j));
                }

                __WriteMsgToRing(byChip,RcvMsg);
                //release the fifo
                if(byFiFoNo==0)
                    pCan->RF0R|=CAN_RF0R_RFOM0;
                else
                    pCan->RF1R|=CAN_RF0R_RFOM0;
            }
        }
    }
    if(byFiFoNo==0)
    {
        //All pending mailbox has deal, clear the pending bit
        if(pCan->RF0R&CAN_RF0R_FULL0)
            pCan->RF0R|=CAN_RF0R_FULL0;
        if(pCan->RF0R&CAN_RF0R_FOVR0)
            pCan->RF0R|=CAN_RF0R_FOVR0;
    }
    else
    {
        if(pCan->RF1R&CAN_RF0R_FULL0)
            pCan->RF1R|=CAN_RF0R_FULL0;
        if(pCan->RF1R&CAN_RF0R_FOVR0)
            pCan->RF1R|=CAN_RF0R_FOVR0;
    }
}

//----CAN总线错误相关的中断处理---------------------------------------------------------
//功能: CAN总线错误相关的中断处理
//参数: pCanPara,CAN控制器配置参数结构体指针;
//      bufid,专用Tx buffer编号
//返回: 空
//-----------------------------------------------------------------------------
static void __CAN_Err_Handle(uint8_t byChip)
{
    tagCanReg *pCan;
    vu32 byEsr;
    uint32_t byLastErrCode;
    bool_t ret;
    if(byChip==0)
    {
      pCan=(tagCanReg *)CN_CAN1_BASE_ADDR;
    }
    else if(byChip==1)
    {
      pCan=(tagCanReg *)CN_CAN3_BASE_ADDR;
    }
    else
        return ;
    pCan->MSR|=CAN_MSR_ERRI;   //clear err interrupt pending bit
    byEsr=pCan->ESR;
    //Bus off
    if(byEsr&CAN_ESR_BOFF)
    {
        gs_u64BusOffCnt[byChip]++;
        s_bBusOffFlag[byChip]=true;
    }
    if(byEsr&CAN_ESR_EPVF)
    {
        gs_u64PassiveErrCnt[byChip]++;
    }
    byLastErrCode=byEsr&(uint32_t)CAN_ESR_LEC;
    if(byLastErrCode!=0)
    {
        switch(byLastErrCode)
        {
           case CAN_ESR_LEC_0:  //
               gs_u64StErrCnt[byChip]++;
               break;
           case CAN_ESR_LEC_1:
               gs_u64FoErrCnt[byChip]++;
               break;
           case (CAN_ESR_LEC_1 | CAN_ESR_LEC_0):
               gs_u64ACKErrCnt[byChip]++;
               break;
           case CAN_ESR_LEC_2:
               gs_u64DomBitErrCnt[byChip]++;
               break;
           case (CAN_ESR_LEC_2 | CAN_ESR_LEC_0):
               gs_u64PasBitErrCnt[byChip]++;
               break;
           case (CAN_ESR_LEC_2 | CAN_ESR_LEC_1):
               gs_u64CrcErrCnt[byChip]++;
               break;
           default:
               break;
        }
    }
    pCan->MSR|=CAN_MSR_ERRI;   //clear err interrupt pending bit
}
//----CAN中断响应函数------------------------------------------------------------
//功能: CAN中断响应函数
//参数: 无
//返回: 空
//-----------------------------------------------------------------------------
static void CAN_ISR_Handler(ptu32_t IntLine)
{
    if(IntLine==CN_INT_LINE_CAN1_RX0)
    {
        Int_ClearLine(CN_INT_LINE_CAN1_RX0);
        gs_u64IntCnt[0]++;
        __CAN_Recv_FiFo_Handle(0,0);
    }
    if(IntLine==CN_INT_LINE_CAN1_RX1)
    {
        Int_ClearLine(CN_INT_LINE_CAN1_RX1);
        gs_u64IntCnt[1]++;
        __CAN_Recv_FiFo_Handle(0,1);
    }

    if(IntLine==CN_INT_LINE_CAN1_SCE)
    {
        Int_ClearLine(CN_INT_LINE_CAN1_SCE);
        gs_u64IntCnt[2]++;
        __CAN_Err_Handle(0);
    }

    if(IntLine==CN_INT_LINE_CAN3_RX0)
    {
        Int_ClearLine(CN_INT_LINE_CAN3_RX0);
        gs_u64IntCnt[3]++;
        __CAN_Recv_FiFo_Handle(1,0);
    }
    if(IntLine==CN_INT_LINE_CAN3_RX1)
    {
        Int_ClearLine(CN_INT_LINE_CAN3_RX1);
        gs_u64IntCnt[4]++;
        __CAN_Recv_FiFo_Handle(1,1);
    }

    if(IntLine==CN_INT_LINE_CAN3_SCE)
    {
        Int_ClearLine(CN_INT_LINE_CAN3_SCE);
        gs_u64IntCnt[5]++;
        __CAN_Err_Handle(1);
    }
}



//----监测CAN通信数据收发情况---------------------------------------------------------
//功能: 监测CAN通信数据收发情况，当2s中CAN控制器收或者发报文数没有变化时，即可认为CAN
//控制器出现故障，此时需要将CAN控制器进行复位操作.
//参数: 无.
//返回: 无.
//-----------------------------------------------------------------------------
ptu32_t __Can_Monitor(void)
{
   s64 sndcnt[CN_CAN_NUM]={0};
   s64 rcvcnt[CN_CAN_NUM]={0};
   uint8_t i;
   bool_t ret;

   while(1)
   {
         for(i=0;i<CN_CAN_NUM;i++)
         {
              if(s_bBusOffFlag[i])
             {
                 DJY_EventDelay(CN_CAN_BUS_OFF_RECOVERY_TIME);
                 s_bBusOffFlag[i]=false;
                 CAN_Hard_Init(i,gs_CANBaudRate[i],&gs_ptagCanFilterPara[i]);

             }
         }

       for(i=0;i<CN_CAN_NUM;i++)
       {
           sndcnt[i]=gs_u64HardSndCnt[i];
           rcvcnt[i]=gs_u64HardRcvCnt[i];
           if((sndcnt[i]!=0)||(rcvcnt[i]!=0))
           {
              bQiYongFlag[i]=true;  //表示该CAN总线被启动了，没有启动的总线无需监测。
           }
       }
       DJY_EventDelay(CN_CAN_DEV_MONITOR_TIME);
       for(i=0;i<CN_CAN_NUM;i++)
       {
           if(bQiYongFlag[i])
           {
             if((sndcnt[i]==gs_u64HardSndCnt[i])&&(rcvcnt[i]==gs_u64HardRcvCnt[i]))
             {
                 gs_MonitorErrRstCnt[i]++;
                 CAN_Hard_Init(i,gs_CANBaudRate[i],&gs_ptagCanFilterPara[i]);
             }

           }
       }
   }

   return 1;
}
//----CAN发送线程---------------------------------------------------------
//功能: 异步发送CAN数据，判断是否发送消息队列中是否有数据，如果有数据则将数据发送出去。
//参数: 无.
//返回: 无.
//-----------------------------------------------------------------------------
ptu32_t __CAN_SndTask(void)
{
      uint8_t byCanNo;
      uint8_t buf[14];
      while(1)
      {
         MsgQ_Receive(gs_ptCanSndMsgQ, buf, CN_CAN_MSGQ_LEN, CN_TIMEOUT_FOREVER);
         byCanNo=buf[0];
         if(byCanNo<CN_CAN_NUM)
         {
              __CAN_HardTxOneFrame(byCanNo,(uint8_t *)&buf[1]);
         }
      }
}
//----初始化CAN配置参数---------------------------------------------------------
//功能: 初始化CAN配置参数
//参数: pCanPara,CAN控制器配置参数结构体指针
//返回: 成功初始化则返回true,否则则返回false.
//-----------------------------------------------------------------------------
bool_t CAN_Main(uint8_t baudrate)
{
    uint16_t evtt_id,event_id;
    uint8_t i;
    bool_t ret;
    for(i=0;i<CN_CAN_NUM;i++)
    {
        gs_ptagCanFilterPara[i].FilterNumber=0;
        gs_ptagCanFilterPara[i].FilterMode=CAN_FILTERMODE_IDMASK;
        gs_ptagCanFilterPara[i].FilterScale=CAN_FILTERSCALE_32BIT;
        gs_ptagCanFilterPara[i].FilterIdHigh=0x0000;
        gs_ptagCanFilterPara[i].FilterIdLow=0x0000;
        gs_ptagCanFilterPara[i].FilterMaskIdHigh=0x0000;
        gs_ptagCanFilterPara[i].FilterMaskIdLow=0x0000;
        gs_ptagCanFilterPara[i].FilterFIFOAssignment=0;
        gs_ptagCanFilterPara[i].FilterActivation=1;
        gs_ptagCanFilterPara[i].BankNumber=14;
        ret=CAN_Hard_Init(i,baudrate,&gs_ptagCanFilterPara[i]);
        if(!ret)
        {
            error_printf("bus", "can bus controller initialization failed.");
            return false;
        }
    }
    gs_ptCanSndMsgQ=MsgQ_Create(CN_CAN_MSGQ_NUM,CN_CAN_MSGQ_LEN,CN_MSGQ_TYPE_FIFO);
    if(gs_ptCanSndMsgQ==NULL)
        return false;
    evtt_id = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,__Can_Monitor,
              CAN_MonitorStack,sizeof(CAN_MonitorStack),"CAN Monitor function");
    if(evtt_id!=CN_EVTT_ID_INVALID)
    {
       event_id=DJY_EventPop(evtt_id,NULL,0,NULL,0,0);
    }
    else
    {
        DJY_EvttUnregist(evtt_id);
        error_printf("bus", "can bus's monitor event pop failed.");
    }
    evtt_id = DJY_EvttRegist(EN_CORRELATIVE,100,0,0,__CAN_SndTask,
              CAN_SndStack,sizeof(CAN_SndStack),"CAN Snd Task");
    if(evtt_id!=CN_EVTT_ID_INVALID)
    {
         event_id=DJY_EventPop(evtt_id,NULL,0,NULL,0,0);
    }
    else
    {
        DJY_EvttUnregist(evtt_id);
        error_printf("bus", "can bus's monitor event pop failed.");
    }
    info_printf("bus", "can bus installed.");
    return true;
}
ADD_TO_ROUTINE_SHELL(canstat,canstat,"CAN通信统计    COMMAND:canstat+enter");
ADD_TO_ROUTINE_SHELL(canrst,canrst,"复位CAN控制器   COMMAND:canrst+CAN控制器编号+enter  ");
ADD_TO_ROUTINE_SHELL(canreg,canreg,"读取CAN寄存器值   COMMAND:canreg+enter");
ADD_TO_ROUTINE_SHELL(canf,canf,"配置CAN过滤器   COMMAND:canf+CAN控制器编号+配置参数列表+enter");
ADD_TO_ROUTINE_SHELL(canb,canb,"设置CAN控制器波特率   COMMAND:cansb+CAN控制器编号+波特率值+enter");
ADD_TO_ROUTINE_SHELL(cands,cands,"设置CAN调试标识符   COMMAND:cands+enter");
ADD_TO_ROUTINE_SHELL(candc,candc,"清CAN调试标识符   COMMAND:candc+enter");
