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

// �ļ���     ��cpu_peri_can.c
// ģ������: MCAN����
// ģ��汾: V1.00
// ������Ա: zhb
// ����ʱ��: 05/01.2017
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

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern bool_t CAN_Main(uint8_t baudrate);
//  CAN_Main(CFG_CPU_CAN_BAUDRATE);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip can"//CPU��can��������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_CAN == false )
//#warning  " cpu_onchip_can  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_CAN    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,512,
#define CFG_CPU_CAN_BAUDRATE   125       //"CAN BaudRate",
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,

#endif
//%$#@end configue  ****�������ý���
//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������
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
static u64 gs_u64PasBitErrCnt[CN_CAN_NUM];  //����λ����
static u64 gs_u64DomBitErrCnt[CN_CAN_NUM];  //����λ����
static u64 gs_u64FoErrCnt[CN_CAN_NUM];      //format err
static u64 gs_u64StErrCnt[CN_CAN_NUM];      //bit stuff err
static u64 gs_u64ACKErrCnt[CN_CAN_NUM];     //ACK err
static u64 gs_MonitorErrRstCnt[CN_CAN_NUM];
static u64 gs_u64RcvPkgBadCnt[CN_CAN_NUM];
static u64 gs_u64SndPkgBadCnt[CN_CAN_NUM];//���ݰ�����������Ч���ݲ�Ϊ8���ֽ�
static u64 gs_u64RstFailedCnt[CN_CAN_NUM];
static u64 gs_u64IntCnt[6];
static uint8_t CAN_MonitorStack[0x1000];
static uint8_t CAN_SndStack[0x1000];
static bool_t gs_CanDebugFlag=false;
static bool_t s_bBusOffFlag[CN_CAN_NUM];
static struct MsgQueue * gs_ptCanSndMsgQ;
bool_t bQiYongFlag[CN_CAN_NUM];

/*---------------------------------------------------------------------------
 * ��������
 *---------------------------------------------------------------------------*/
static void CAN_ISR_Handler(ptu32_t IntLine);
bool_t CAN_Hard_Init(uint8_t byCanNo,uint32_t baudrate,tagCanFilterPara \
        * pFilterConfPara);

/*******************************************************************************
����:��λCAN��������
����:�ޡ�
���:�ޡ�
*********************************************************************************/
//static bool_t Sh_CAN_Reset(char *param)
bool_t canrst(char *param)
{
    char *word_ChipNum,*word_trail,*next_param;
    uint8_t byCanNo;
    bool_t ret;
    //��ȡ1������
    word_ChipNum = shell_inputs(param,&next_param);
    word_trail = shell_inputs(next_param,&next_param);
    if((word_ChipNum == NULL)||(word_trail != NULL))
    {
      printf("\r\n��ʽ������ȷ��ʽ�ǣ�\r\n>d CAN���������. \r\n");
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
����:��ȡCAN����ͨ��ͳ������
����:�ޡ�
���:�ޡ�
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
����:��ȡCAN�ɶ��Ĵ�����ֵ��
����:��.
���:�ޡ�
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
����:CAN���ƿ��ڴ������ʼ����
����:�ա�
����:true--��ʼ���ɹ���false--��ʼ��ʧ�ܡ�
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
      printf("\r\nCAN��������������.\r\n");
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
����:CAN���������ò����ʡ�
����:�ա�
����:true--���óɹ���false--����ʧ�ܡ�
******************************************************************************/
//static void Sh_CAN_Set_BaudRate(char *param)
bool_t canb(char *param)
{
    char *word_ChipNum,*word_BaudRate,*word_trail,*next_param;
    uint8_t byCanNo;
    uint32_t BaudRate;
    //��ȡ1������

    word_ChipNum = shell_inputs(param,&next_param);
    word_BaudRate = shell_inputs(next_param,&next_param);
    word_trail = shell_inputs(next_param,&next_param);
    if((word_ChipNum == NULL)||(word_BaudRate==NULL)||(word_trail != NULL))
    {
      printf("\r\n��ʽ������ȷ��ʽ�ǣ�\r\n>d CAN���������+������ֵ+Enter. \r\n");
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
����:��λCAN���Ա�ʶ����
����:�ա�
����:�ޡ�
******************************************************************************/
//static void Sh_CAN_Set_DebugFlag(char *param)
bool_t cands(char *param)
{
    gs_CanDebugFlag=true;
    return true;
}

/*******************************************************************************
����:��CAN���Ա�ʶ����
����:�ա�
����:�ޡ�
******************************************************************************/
//static void Sh_CAN_Clear_DebugFlag(char *param)
bool_t candc(char *param)
{
    gs_CanDebugFlag=false;
    return true;
}
/*******************************************************************************
����:CAN���ƿ��ڴ������ʼ����
����:�ա�
����:true--��ʼ���ɹ���false--��ʼ��ʧ�ܡ�
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
//----CAN�˲�������------------------------------------------------------------
//����:����CAN������ �˲����ܣ���ͨ�������˲���������CAN������ֻ����ָ��ID��CAN���ģ���������ֱ�Ӷ���.
//����: byChip,CAN���������;
//     pFilterConfPara:CAN�˲������ò���
//����: �ɹ�����true,���򷵻�false.
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
//----CAN����������------------------------------------------------------------
//����: ����CANͨ�Ų�����
//����: byChip,CAN���������;
//     baudrate,�����ʣ�ֻ֧��125k��250k����.
//����: �ɹ��򷵻�true,���򷵻�false.
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

//----CAN�˲�������------------------------------------------------------------
//����: ��ʼ��CAN�ж�����
//����: pCanPara,CAN���������ò����ṹ��ָ��
//����: �ɹ���ʼ���򷵻�true,�����򷵻�false.
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
//----��ʼ��CAN�ж�����---------------------------------------------------------
//����: ��ʼ��CAN�ж�����
//����: pCanPara,CAN���������ò����ṹ��ָ��
//����: �ɹ���ʼ���򷵻�true,�����򷵻�false.
//-----------------------------------------------------------------------------

static void  __CAN_IntInit(uint8_t byCanNo)
{
    //���ж���
    if(byCanNo==0)
    {
        //�����ж���
        Int_Register(CN_INT_LINE_CAN1_RX0);
        Int_IsrConnect(CN_INT_LINE_CAN1_RX0,(uint32_t(*)(ptu32_t))CAN_ISR_Handler);
        Int_SettoAsynSignal(CN_INT_LINE_CAN1_RX0);
        Int_ClearLine(CN_INT_LINE_CAN1_RX0);
        Int_RestoreAsynLine(CN_INT_LINE_CAN1_RX0);
        //�����ж���
        Int_Register(CN_INT_LINE_CAN1_RX1);
        Int_IsrConnect(CN_INT_LINE_CAN1_RX1,(uint32_t(*)(ptu32_t))CAN_ISR_Handler);
        Int_SettoAsynSignal(CN_INT_LINE_CAN1_RX1);
        Int_ClearLine(CN_INT_LINE_CAN1_RX1);
        Int_RestoreAsynLine(CN_INT_LINE_CAN1_RX1);
        //�����ж���
        Int_Register(CN_INT_LINE_CAN1_SCE);
        Int_IsrConnect(CN_INT_LINE_CAN1_SCE,(uint32_t(*)(ptu32_t))CAN_ISR_Handler);
        Int_SettoAsynSignal(CN_INT_LINE_CAN1_SCE);
        Int_ClearLine(CN_INT_LINE_CAN1_SCE);
        Int_RestoreAsynLine(CN_INT_LINE_CAN1_SCE);
    }
    else
    {
        //�����ж���
        Int_Register(CN_INT_LINE_CAN3_RX0);
        Int_IsrConnect(CN_INT_LINE_CAN3_RX0,(uint32_t(*)(ptu32_t))CAN_ISR_Handler);
        Int_SettoAsynSignal(CN_INT_LINE_CAN3_RX0);
        Int_ClearLine(CN_INT_LINE_CAN3_RX0);
        Int_RestoreAsynLine(CN_INT_LINE_CAN3_RX0);
        //�����ж���
        Int_Register(CN_INT_LINE_CAN3_RX1);
        Int_IsrConnect(CN_INT_LINE_CAN3_RX1,(uint32_t(*)(ptu32_t))CAN_ISR_Handler);
        Int_SettoAsynSignal(CN_INT_LINE_CAN3_RX1);
        Int_ClearLine(CN_INT_LINE_CAN3_RX1);
        Int_RestoreAsynLine(CN_INT_LINE_CAN3_RX1);
        //�����ж���
        Int_Register(CN_INT_LINE_CAN3_SCE);
        Int_IsrConnect(CN_INT_LINE_CAN3_SCE,(uint32_t(*)(ptu32_t))CAN_ISR_Handler);
        Int_SettoAsynSignal(CN_INT_LINE_CAN3_SCE);
        Int_ClearLine(CN_INT_LINE_CAN3_SCE);
        Int_RestoreAsynLine(CN_INT_LINE_CAN3_SCE);
    }

}

//----����CANͨ�Ų�����---------------------------------------------------------
//����: ����CANͨ�Ų����ʡ�
//����: pCan,CAN�������Ĵ����ṹ��ָ�룻
//      baudrate,CANͨ�Ų����ʡ�
//����: �ɹ�����true,ʧ�ܷ���false.
//˵�����ڳ��׮Ӧ���н�����250k��125k��ͨ�Ų����ʣ�Ŀǰ��֧�������ֲ�����.CAN1/CAN2
//      ʱ��Դ������APB1,��sysinit.c���ѽ�APB1��ʱ������Ϊ54MHz.
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
����:CAN�豸�ĳ�ʼ����������ʼ���жϿ������������ڴ棬��ʼ��������
����:byIndex--CAN��������ţ�0~1.
���:true--��ʼ���ɹ���false-��ʼ��ʧ�ܡ�
˵����
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
        //��ʼ�����ջ��λ�����
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

    //Set the bit timing register  APB1�� 54MHz
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

//----ѡ��һ�����еķ�������-------------------------------------
//����: �ӱ��0��ʼ���β�ѯ�������������״̬�Ƿ�Ϊ�գ�ֱ���ҵ��ҵ�һ�������䣬���
//      3�������������Ϊ�գ��򷵻�-1.
//����: byChip:  CAN���������
//����: ���3�������������Ϊ�գ��򷵻�-1.��������Ӧ�ķ���0��1��2.
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

//----���ĳ��ָ�����������Ƿ�ɹ������ݷ��ͳ�ȥ-------------------------------------
//����: ���ĳ��ָ�����������Ƿ�ɹ������ݷ��ͳ�ȥ��
//����: TxMailboxNo:  ���������ţ�0~2.
//����: ������ͳɹ�����true,ʧ���򷵻�false.
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
//----CAN���ͺ���-------------------------------------
//����: CAN���ͺ���
//����:
       // byChip:  CAN���������   0~1(Ϊ������Ŀ����룬�����˲�������SR5333�иò���
//        Ϊ0��SR5333 V1.01�汾��ֻʹ����һ��CAN������)
       // txBuf:   ���ĵķ��ͻ�����
       // len:     ���ͱ��ĵĳ���,13��������.
//����: �ɹ����͵ĳ���
//-----------------------------------------------------------------------------
uint32_t CAN_WriteData(uint8_t byChip, uint8_t* txBuf, uint32_t len)
{

    uint8_t pkgnum,i,j;
    bool_t ret;
    uint8_t buf[14];
    //�������
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


//----�ײ�CAN�첽����һ֡CAN����-------------------------------------
//����: CAN���ͺ���
//����:
       // byChip:  CAN���������   0~1(Ϊ������Ŀ����룬�����˲�������SR5333�иò���
//        Ϊ0��SR5333 V1.01�汾��ֻʹ����һ��CAN������)
       // txBuf:   ���ĵķ��ͻ�����
//����: �ɹ����ͷ���true,ʧ���򷵻�false.
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
    //��ѯ�Ƿ�ɹ����ͳ�ȥ
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

//----CAN���պ���-------------------------------------
//����: CAN���ͺ���
//����:
      // byChip:  CAN������ѡ��   0~4(Ϊ������Ŀ����룬�����˲�������SR5333�иò���
      //  Ϊ0��SR5333 V1.01�汾��ֻʹ����һ��CAN������)
      // txBuf:    ���ĵĽ��ջ�����
      // len:     �����ȡ�ĳ���
      // pRd:     ��������ָ��
//����: �ɹ���ȡ���ݵĳ���
//-----------------------------------------------------------------------------
uint32_t CAN_ReadData(uint8_t byChip, uint8_t* rxBuf, uint32_t len, uint32_t *pRd)
{
    uint32_t rdLen=0x0000,ringlen,pkgnum,r_len;
    CAN_DevCtrl *CAN_DevCtrlTempptr=NULL;
    struct RingBuf *rxRingBuf=NULL;
    atom_low_t atom;
    //�����������
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

//----��CANӲ�����յ�������д�����λ�������---------------------------------------------------------
//����: ��CANӲ�����յ�������д�����λ������У�ÿ֡�򻺳���д��13bytes.
//����: rxMsg,CAN�������ݽṹ��;
//����: ��
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
//----�����ж���Ӧ����---------------------------------------------------------
//����: ���β�ѯר�ý���Buffer(Dedicated RX Buffer)�������������
//����: byChip,CAN��������ţ�0��1;
//      byFiFoNo,����FIFO��ţ�0��1.
//����: ��
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

//----CAN���ߴ�����ص��жϴ���---------------------------------------------------------
//����: CAN���ߴ�����ص��жϴ���
//����: pCanPara,CAN���������ò����ṹ��ָ��;
//      bufid,ר��Tx buffer���
//����: ��
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
//----CAN�ж���Ӧ����------------------------------------------------------------
//����: CAN�ж���Ӧ����
//����: ��
//����: ��
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



//----���CANͨ�������շ����---------------------------------------------------------
//����: ���CANͨ�������շ��������2s��CAN�������ջ��߷�������û�б仯ʱ��������ΪCAN
//���������ֹ��ϣ���ʱ��Ҫ��CAN���������и�λ����.
//����: ��.
//����: ��.
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
              bQiYongFlag[i]=true;  //��ʾ��CAN���߱������ˣ�û�����������������⡣
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
//----CAN�����߳�---------------------------------------------------------
//����: �첽����CAN���ݣ��ж��Ƿ�����Ϣ�������Ƿ������ݣ���������������ݷ��ͳ�ȥ��
//����: ��.
//����: ��.
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
//----��ʼ��CAN���ò���---------------------------------------------------------
//����: ��ʼ��CAN���ò���
//����: pCanPara,CAN���������ò����ṹ��ָ��
//����: �ɹ���ʼ���򷵻�true,�����򷵻�false.
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
ADD_TO_ROUTINE_SHELL(canstat,canstat,"CANͨ��ͳ��    COMMAND:canstat+enter");
ADD_TO_ROUTINE_SHELL(canrst,canrst,"��λCAN������   COMMAND:canrst+CAN���������+enter  ");
ADD_TO_ROUTINE_SHELL(canreg,canreg,"��ȡCAN�Ĵ���ֵ   COMMAND:canreg+enter");
ADD_TO_ROUTINE_SHELL(canf,canf,"����CAN������   COMMAND:canf+CAN���������+���ò����б�+enter");
ADD_TO_ROUTINE_SHELL(canb,canb,"����CAN������������   COMMAND:cansb+CAN���������+������ֵ+enter");
ADD_TO_ROUTINE_SHELL(cands,cands,"����CAN���Ա�ʶ��   COMMAND:cands+enter");
ADD_TO_ROUTINE_SHELL(candc,candc,"��CAN���Ա�ʶ��   COMMAND:candc+enter");
