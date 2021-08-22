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
// ģ������:exception.c
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա:Administrator
// ����ʱ��:2013-7-26--����10:15:29
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// ============================================================================
#include "stdint.h"
#include "stdio.h"
#include "endian.h"
#include "exp.h"
#include "hard-exp.h"
#include <shell.h>

//#include "cpu_peri.h"

//#include <device/djy_uart.h>
//#include "flash_s29glxxx.h"
//#include "shell.h"

#include <cdef21469.h>
#include <def21489.h>
#include <sru.h>
#include <sysreg.h>

extern struct IntLine *tg_pIntLineTable[];       //�ж��߲��ұ�
bool_t  HardExp_Decoder(struct ExpThrowPara *parahead, u32 endian);

extern exp_stack_space;      // The base of the exception stack
extern exp_stack_length;    // The size of the exception stack
extern exp_stack_end;       // The end of the exception stack

//#define Exception_StartAddress   (  (exp_stack_space + exp_stack_length - 1)
//                                          - ((exp_stack_space + exp_stack_length - 1) % 2) )

#define  CN_HARDEXP_INFODECODERNAME    "HARDEXP_DECODER"
#define  CN_SYSEXP_UNIVERSAL_LEN         ((u32)0x8b)
#define  CN_SYSEXP_SPORTSPECIAL_LEN      ((u32)0x69)
#define  CN_SYSEXP_SOVFI_MAX_LEN         ((u32)0x5d)

/*******************************************************************
*  function prototypes
*******************************************************************/

extern void __asm_exp_init(void);
extern void __asm_software_reset(void);

extern bool_t g_bScheduleEnable;



/* function prototypes */
enum EN_ExpAction Exp_General(u32 major,u32 minor,u32 exp_len);
bool_t __Exp_InstructionDecoder(u32 *exp_addr);
bool_t __Exp_HardStackDecoder(u32 *exp_addr,u32 exp_len);
bool_t __Exp_SportDecoder(u32 *exp_addr,u32 exp_len);
bool_t __Exp_CalculateDecoder(u32 *exp_addr,u32 exp_len);
bool_t __Exp_UniversalDecoder(u32 *exp_addr,u32 exp_len);
bool_t __Exp_SpecialDecoder(u32 *exp_addr,u32 major,u32 exp_len);
void   __Exp_SwapCpuInfoByEndian(u32 *pAddr,u32 exp_len);
bool_t  Exp_Harddecoder(struct ExpThrowPara *parahead, u32 endian);
void HardExp_Init(void);

bool_t Sh_SoftwareReset(char *param);
bool_t Sh_Reboot(char *param);
bool_t Sh_RestartSystem(char *param);

//��������쳣��������
static char *s_pcMajorExpNameTab[CN_MAJOREXP_INVALID] =
{
    "IICDI",
    "SOVFI",
    "SPERRI",
    "CB7I",
    "CB15I",
    "FIXI",
    "FLTOI",
    "FLTUI",
    "FLTII",
    "Invalid Exception"
};

enum _EN_SYSEXP_CPU_Major_
{
    EN_SYSEXP_MAJOR_IICDI=0,
    EN_SYSEXP_MAJOR_SOVFI,
    EN_SYSEXP_MAJOR_SPERRI,
    EN_SYSEXP_MAJOR_BKPI,
    EN_SYSEXP_MAJOR_CB7I,
    EN_SYSEXP_MAJOR_CB15I,
    EN_SYSEXP_MAJOR_FIXI,
    EN_SYSEXP_MAJOR_FLTOI,
    EN_SYSEXP_MAJOR_FLTUI,
    EN_SYSEXP_MAJOR_FLTII,
    EN_SYSEXP_MAJOR_EMULI,
    EN_SYSEXP_MAJOR_INVALID_EXCEPTION,
};


static char *s_pcMinorExpNameTab[CN_MINOREXP_INVALID]=
{
    "Illegal_IOP_Access",
    "Unaligned_Long_Word_Access ",
    "PC_Full",
    "Status_Overflow",
    "Loop_Overflow",
    "SPORT0_Unexpected_Frame_Syncs",
    "SPORT0_A_Data_Error ",
    "SPORT0_B_Data_Error ",
    "SPORT1_Unexpected_Frame_Syncs",
    "SPORT1_A_Data_Error ",
    "SPORT1_B_Data_Error ",
    "SPORT2_Unexpected_Frame_Syncs",
    "SPORT2_A_Data_Error ",
    "SPORT2_B_Data_Error ",
    "SPORT3_Unexpected_Frame_Syncs",
    "SPORT3_A_Data_Error ",
    "SPORT3_B_Data_Error ",
    "SPORT4_Unexpected_Frame_Syncs",
    "SPORT4_A_Data_Error ",
    "SPORT4_B_Data_Error ",
    "SPORT5_Unexpected_Frame_Syncs",
    "SPORT5_A_Data_Error ",
    "SPORT5_B_Data_Error ",
    "SPORT6_Unexpected_Frame_Syncs",
    "SPORT6_A_Data_Error ",
    "SPORT6_B_Data_Error ",
    "SPORT7_Unexpected_Frame_Syncs",
    "SPORT7_A_Data_Error ",
    "SPORT7_B_Data_Error ",
    "I7 overflow",
    "I15 overflow",
    "ALU_Fixed_Overflow",
    "Multiplier_Fixed_Overflow",
    "ALU_Floating_Overflow",
    "Multiplier_Floating_Overflow",
    "ALU_Floating_Underflow",
    "Multiplier_Floating_Underflow",
    "ALU_Floating_Invalid",
    "Multiplier_Floating_Invalid",
    "Invalid Exception"
};


enum __EN_SYSEXP_CPU_Minor
{
    EN_SYSEXP_ILLEGAL_IOP_ACCESS     =1,
    EN_SYSEXP_PC_FULL,
    EN_SYSEXP_MINOR_PC_FULL,
    EN_SYSEXP_STATUS_OVERFLOW,
    EN_SYSEXP_LOOP_OVERFLOW,
    EN_SYSEXP_SP0_UNEXPECTED_FRAME_SYNCS,
    EN_SYSEXP_SP0_A_DATA_ERROR,
    EN_SYSEXP_SP0_B_DATA_ERROR,
    EN_SYSEXP_SP1_UNEXPECTED_FRAME_SYNCS,
    EN_SYSEXP_SP1_A_DATA_ERROR,
    EN_SYSEXP_SP1_B_DATA_ERROR,
    EN_SYSEXP_SP2_UNEXPECTED_FRAME_SYNCS,
    EN_SYSEXP_SP2_A_DATA_ERROR,
    EN_SYSEXP_SP2_B_DATA_ERROR,
    EN_SYSEXP_SP3_UNEXPECTED_FRAME_SYNCS,
    EN_SYSEXP_SP3_A_DATA_ERROR,
    EN_SYSEXP_SP3_B_DATA_ERROR,
    EN_SYSEXP_SP4_UNEXPECTED_FRAME_SYNCS,
    EN_SYSEXP_SP4_A_DATA_ERROR,
    EN_SYSEXP_SP4_B_DATA_ERROR,
    EN_SYSEXP_SP5_UNEXPECTED_FRAME_SYNCS,
    EN_SYSEXP_SP5_A_DATA_ERROR,
    EN_SYSEXP_SP5_B_DATA_ERROR,
    EN_SYSEXP_SP6_UNEXPECTED_FRAME_SYNCS,
    EN_SYSEXP_SP6_A_DATA_ERROR,
    EN_SYSEXP_SP6_B_DATA_ERROR,
    EN_SYSEXP_SP7_UNEXPECTED_FRAME_SYNCS,
    EN_SYSEXP_SP7_A_DATA_ERROR,
    EN_SYSEXP_SP7_B_DATA_ERROR,
    EN_SYSEXP_I7_OVERFLOW,
    EN_SYSEXP_I15_OVERFLOW,
    EN_SYSEXP_ALU_FIXED_OVERFLOW,
    EN_SYSEXP_MUL_FIXED_OVERFLOW,
    EN_SYSEXP_ALU_FLOAT_OVERFLOW,
    EN_SYSEXP_MUL_FLOAT_OVERFLOW,
    EN_SYSEXP_ALU_FLOAT_UNDERFLOW,
    EN_SYSEXP_MUL_FLOAT_UNDERFLOW,
    EN_SYSEXP_MUL_FLOAT_INVALID,
    EN_SYSEXP_ALU_FLOAT_INVALID,
    EN_SYSEXP_MINOR_INVALID_EXCEPTION,

};


// =============================================================================
// �������ܣ�exception_generaldealer
//           �˲�����Ҫ��������CPU�쳣ͨ�ýӿڣ������쳣���ͣ������Ƿ��׳��쳣��
// ���������u32 major,�쳣����ţ�
//           u32 minor���쳣��֧��ţ�
//           u32 exp_len,�쳣��Ϣ����
// �����������
// ����ֵ  ��EN_ExpAction
// =============================================================================
enum EN_ExpAction Exp_General(u32 major,u32 minor,u32 exp_len)
{
    char *majorname;
    struct ExpThrowPara  head;
    enum EN_ExpAction Action;


    //make sure not shedule
    bool_t   shedule_bak;
    shedule_bak = g_bScheduleEnable ;
    g_bScheduleEnable = false;

    //�����쳣����
    //���쳣�����ദ���ּ��������ʱ��ֱ�Ӹ�λ����CPU���봦����쳣
    if(major>= EN_SYSEXP_MAJOR_INVALID_EXCEPTION||minor>=EN_SYSEXP_MINOR_INVALID_EXCEPTION)
    {
        major = EN_SYSEXP_MAJOR_INVALID_EXCEPTION;
        minor = EN_SYSEXP_MINOR_INVALID_EXCEPTION;
        majorname = s_pcMajorExpNameTab[major];
        printk("This exception is invalid!\n\r");
        //����CORE ��Ϣ��PERI��Ϣ
        result = EN_BLACKBOX_DEAL_RESET;
    }
    else
    {
        majorname = s_pcMajorExpNameTab[major];
        //printk("Exception:%s\n\r",majorname);


     //��������Ҫ���ݾ����쳣����ȷ���쳣������(result)
     switch (major)
     {
      case EN_SYSEXP_MAJOR_IICDI:
      result=EN_BLACKBOX_DEAL_RECORD;
      break;
      case EN_SYSEXP_MAJOR_SOVFI:
      result=EN_BLACKBOX_DEAL_RESTART;
      break;
      case EN_SYSEXP_MAJOR_SPERRI:
      result=EN_BLACKBOX_DEAL_RESTART;
      break;
      case EN_SYSEXP_MAJOR_CB7I:
      result=EN_BLACKBOX_DEAL_RESTART;
      break;
      case EN_SYSEXP_MAJOR_CB15I:
      result=EN_BLACKBOX_DEAL_RESTART;
      break;
      case EN_SYSEXP_MAJOR_FIXI:
      result=EN_BLACKBOX_DEAL_RESTART;
      break;
      case EN_SYSEXP_MAJOR_FLTOI:
      result=EN_BLACKBOX_DEAL_RESTART;
      break;
      case EN_SYSEXP_MAJOR_FLTUI:   //FLTUI�쳣ֻ���¼������Ҫ��λ��
      result=EN_BLACKBOX_DEAL_RECORD;
      break;
      case EN_SYSEXP_MAJOR_FLTII:
      result=EN_BLACKBOX_DEAL_RESTART;
      break;
      default:
      result=EN_BLACKBOX_DEAL_RESTART;
      break;

     }
    }

    head.ExpAction=result;
    head.DecoderName = CN_HARDEXP_INFODECODERNAME;
    head.ExpInfo =(u8*)(Exception_StartAddress-5);
    head.ExpInfoLen =exp_len;
    head.ExpType=0;     //TODO: ����Ҫ�޸�
    Action = Exp_Throw(&head);
    //recovethe shedule
    g_bScheduleEnable = shedule_bak;


    return Action;
}


// =============================================================================
// �������ܣ�_ExpInstructionDecoder
//           �˲���������� IICDI/CB7I/CB15I�쳣���������쳣��ַ��ǰ��50��ָ�
// ���������u32 *exp_addr���洢ָ���ַ
// �����������
// ����ֵ  ���ɹ���������true,���󷵻�false��
// =============================================================================
bool_t __Exp_InstructionDecoder(u32 *exp_addr)
{
    u32 i=0;
    printk("The 50 instructions before the address ocuured exception are printed as follwings:\r\n");
    for(i=0;i<50;i++)
    {
        printk("The %02d line is:%d\r\n",i,*(exp_addr+i));
    }
    return true;
}

// =============================================================================
// �������ܣ�_ExpHardStackDecoder
//           �˲�����������Ӳ��ջ����쳣SOVFI��Ӳ��ջ�����ݣ��������Loop Stack
//           Status Stack��PC Stack����ջ��ȼ�ջ�����ݡ�
// ���������u32 *exp_addr���洢ָ���ַ
//           u32 exp_len,   �쳣�洢��Ϣ����(SHARC21469 1���ֽ�=32bit)
// �����������
// ����ֵ  ���ɹ���������true,���󷵻�false��
// =============================================================================
bool_t __Exp_HardStackDecoder(u32 *exp_addr,u32 exp_len)
{
    u32 i=0;
    u32 loop_couter=0;
    u32 sts_couter=0;
    u32 pc_couter=0;
    u32 temp=0;
    if(exp_len>CN_SYSEXP_SOVFI_MAX_LEN)
    {
        printk("Sport Exception info len is inconsistent!\n\r");
        return false; //���ܲ���һ����CPU���߰汾���޷�����
    }

    else
   {
     printk("The HardStack infomation are printed as follwings:\r\n");

     //���Ƚ���Loop Stack��
     loop_couter=*(exp_addr+exp_len-1);
     printk("The Loop Stack ocupied %02d layers\r\n",loop_couter);
    if(loop_couter!=0)
    {
        printk("Start to print Loop Stack.\r\n");
        for(i=1;i<=loop_couter;i++)
        {
          printk("%02d layer,LCNTR:%08x\r\n",   (loop_couter-i+1), *(exp_addr+exp_len-1-3*(i-1)-3));
          printk("%02d layer,LADDR:%08x\r\n",   (loop_couter-i+1), *(exp_addr+exp_len-1-3*(i-1)-2));
          printk("%02d layer,CURLCNTR:%08x\r\n",(loop_couter-i+1), *(exp_addr+exp_len-1-3*(i-1)-1));
        }
    }
    else
    {
         printk("The Loop Stack is empty.\r\n");
    }

    //��ʼ����Status Stack��
    temp=exp_len-1-3*loop_couter-1;
    sts_couter=*(exp_addr+temp);
     printk("The Status Stack ocupied %02d layers\r\n",sts_couter);
      if(sts_couter!=0)
        {
           printk("Start to print Status Stack.\r\n");
           for(i=1;i<=sts_couter;i++)
           {
            printk("%02d layer,MODE1:%08x\r\n", (sts_couter-i+1),*(exp_addr+temp-3*(i-1)-3));
            printk("%02d layer,ASTATx:%08x\r\n",(sts_couter-i+1),*(exp_addr+temp-3*(i-1)-2));
            printk("%02d layer,ASTATy:%08x\r\n",(sts_couter-i+1),*(exp_addr+temp-3*(i-1)-1));
           }
        }
             else
        {
            printk("The Status Stack is empty.\r\n");
        }

        //��ʼ����PC Stack.
        temp=exp_len-1-3*loop_couter-1-3*sts_couter-1;
        pc_couter=*(exp_addr+temp);
        printk("The PC Stack ocupied %02d layers\r\n",pc_couter);

        if(pc_couter!=0)
        {
            printk("Start to print PC Stack.\r\n");
            for(i=1;i<=pc_couter;i++)
            {
              printk("%02d layer, PCSTK:%08x\r\n",(pc_couter-i+1),*(exp_addr+temp-i));
            }
        }
        else
        {
            printk("The PC stack is empty.\r\n");
        }

        printk("Print Hard Stack end.\r\n");

    return true;
  }
}

// =============================================================================
// �������ܣ�_ExpHardStackDecoder
//           �˲�����������Ӳ��ջ����쳣SOVFI��Ӳ��ջ�����ݣ��������Loop Stack
//           Status Stack��PC Stack����ջ��ȼ�ջ�����ݡ�
// ���������u32 *exp_addr���洢ָ���ַ
//           u32 exp_len,   �쳣�洢��Ϣ����(SHARC21469 1���ֽ�=32bit)
// �����������
// ����ֵ  ���ɹ���������true,���󷵻�false��
// =============================================================================
bool_t __Exp_SportDecoder(u32 *exp_addr,u32 exp_len)
{
    u32 i=0;
    u32 j=0;

    if(exp_len!=CN_SYSEXP_SPORTSPECIAL_LEN)
    {
        printk("Sport Exception info len is inconsistent!\n\r");
        return false; //���ܲ���һ����CPU���߰汾���޷�����
    }
    else
    {
     printk("The SPORT related registers printed as follwings:\r\n");
     printk("SPERRSTAT:%08x\r\n",*exp_addr);
    for (i=0;i<=7;i++)
    {
     printk("SPERRCTL%01d:%08x\r\n",i,*(exp_addr+i+1));
    }
    for(i=0;i<=7;i++)
    {
        printk("DIV%01d:%08x\r\n",i,*(exp_addr+9+i));
    }
    for(i=0;i<=7;i++)
    {
        printk("SPCTL%01d:%08x\r\n",i,*(exp_addr+17+i));
    }
    for(i=0;i<=7;i++)
    {
        printk("SPCTLN%01d:%08x\r\n",i,*(exp_addr+25+i));
    }
    for(i=0;i<=7;i++)
    {
        printk("SPMCTL%01d:%08x\r\n",i,*(exp_addr+33+i));
    }
    for(i=0;i<=7;i++)
    {
        for(j=0;j<=3;j++)
        {
            printk("SP%01dCS%01d:%08x\r\n",i,j,*(exp_addr+41+4*i+j));
        }
    }
    for(i=0;i<=7;i++)
    {
        for(j=0;j<=3;j++)
        {
            printk("SP%01dCCS%01d:%08x\r\n",i,j,*(exp_addr+73+4*i+j));
        }
    }
    printk("Print the SPORT related registers end.\r\n");

    return true;
    }

}


// =============================================================================
// �������ܣ�_ExpUniversalDecoder
//           �˲������������쳣��Ϣͨ�üĴ���ֵ��
// ���������u32 *exp_addr���洢ָ���ַ
//           u32 exp_len,   �쳣�洢��Ϣ����(SHARC21469 1���ֽ�=32bit)
// �����������
// ����ֵ  ���ɹ���������true,���󷵻�false��
// =============================================================================

bool_t __Exp_UniversalDecoder(u32 *exp_addr,u32 exp_len)
{
     u32 i =0;
     //u32 *regvalue;
     if(exp_len!=CN_SYSEXP_UNIVERSAL_LEN)
     {
            printk("cpuinfodecoder:incomplete info frame!\n\r");
            return false;
     }
     else
     {
     printk("SysExpInfo:SHARC21469 CORE REGISTERS:\n\r");
     printk("Universal REGISTERS(IN HEX):\n\r");
     //regvalue=(u32 *)exp_addr;

    printk("MODE1:%08x\n\r",*(exp_addr+2));
    printk("MODE2:%08x\n\r",*(exp_addr+3));
    for(i=1;i<5;i++)
    {
        printk("USTAT%d:%08x\n\r",i,*(exp_addr+3+i));
    }
    printk("PCSTK:%08x\n\r",*(exp_addr+0xa));
    printk("ASTATx:%08x\n\r",*(exp_addr+0xb));
    printk("ASTATy:%08x\n\r",*(exp_addr+0xc));
    printk("STKYx:%08x\n\r",*(exp_addr+0xd));
    printk("STKYy:%08x\n\r",*(exp_addr+0xe));
    printk("IRPTL:%08x\n\r",*(exp_addr+0xf));
    printk("LIRPTL:%08x\n\r",*(exp_addr+0x10));
    printk("IMASK:%08x\n\r",*(exp_addr+0x11));

    //Print R0~R15
    printk("PX1:%08x\n\r",*(exp_addr+0x12));
    printk("PX2:%08x\n\r",*(exp_addr+0x13));
    printk("R0(low):%08x\n\r",*(exp_addr+0x14));
    printk("R0(high):%08x\n\r",*(exp_addr+0x15));
    printk("R1:%08x\n\r",*(exp_addr+0x1));
    printk("R2:%08x\n\r",*(exp_addr+0x0));
    printk("R3(low):%08x\n\r",*(exp_addr+0x16));
    printk("R3(high):%08x\n\r",*(exp_addr+0x17));
    printk("R4(low):%08x\n\r",*(exp_addr+0x18));
    printk("R4(high):%08x\n\r",*(exp_addr+0x19));
    printk("R5:%08x\n\r",*(exp_addr+0x1a));
    printk("R6:%08x\n\r",*(exp_addr+0x1b));
    printk("R7(low):%08x\n\r",*(exp_addr+0x1c));
    printk("R7(high):%08x\n\r",*(exp_addr+0x1d));
    printk("R8(low):%08x\n\r",*(exp_addr+0x1e));
    printk("R8(high):%08x\n\r",*(exp_addr+0x1f));
    printk("R9(low):%08x\n\r",*(exp_addr+0x20));
    printk("R9(high):%08x\n\r",*(exp_addr+0x21));
    printk("R10:%08x\n\r",*(exp_addr+0x22));
    printk("R11(low):%08x\n\r",*(exp_addr+0x23));
    printk("R11(high):%08x\n\r",*(exp_addr+0x24));
    printk("R12(low):%08x\n\r",*(exp_addr+0x25));
    printk("R12(high):%08x\n\r",*(exp_addr+0x26));
    printk("R13:%08x\n\r",*(exp_addr+0x27));
    printk("R14:%08x\n\r",*(exp_addr+0x28));
    printk("R15:%08x\n\r",*(exp_addr+0x29));
    //Print S0~S15
    printk("S0(low):%08x\n\r",*(exp_addr+0x2a));
    printk("S0(high):%08x\n\r",*(exp_addr+0x2b));
    printk("S1:%08x\n\r",*(exp_addr+0x2c));
    printk("S2:%08x\n\r",*(exp_addr+0x2d));
    printk("S3:%08x\n\r",*(exp_addr+0x2e));
    printk("S4(low):%08x\n\r",*(exp_addr+0x2f));
    printk("S4(high):%08x\n\r",*(exp_addr+0x30));
    printk("S5:%08x\n\r",*(exp_addr+0x31));
    printk("S6:%08x\n\r",*(exp_addr+0x32));
    printk("S7:%08x\n\r",*(exp_addr+0x33));
    printk("S8:%08x\n\r",*(exp_addr+0x34));
    printk("S9(low):%08x\n\r",*(exp_addr+0x35));
    printk("S9(high):%08x\n\r",*(exp_addr+0x36));
    printk("S10:%08x\n\r",*(exp_addr+0x37));
    printk("S11(low):%08x\n\r",*(exp_addr+0x38));
    printk("S11(high):%08x\n\r",*(exp_addr+0x39));
    printk("S12(low):%08x\n\r",*(exp_addr+0x3a));
    printk("S12(high):%08x\n\r",*(exp_addr+0x3b));
    printk("S13:%08x\n\r",*(exp_addr+0x3c));
    printk("S14:%08x\n\r",*(exp_addr+0x3d));
    printk("S15:%08x\n\r",*(exp_addr+0x3e));
    //Print I0~I15
    for(i=0;i<16;i++)
    {
        printk("I%d:%08x\n\r",i,*(exp_addr+0x3f+i));
    }
    //Print M0~M15
    for(i=0;i<16;i++)
    {
        printk("M%d:%08x\n\r",i,*(exp_addr+0x4f+i));
    }
    //Print B0~B15
    for(i=0;i<16;i++)
    {
        printk("B%d:%08x\n\r",i,*(exp_addr+0x5f+i));
    }
    //Print L0~L15
    for(i=0;i<16;i++)
    {
        printk("L%d:%08x\n\r",i,*(exp_addr+0x6f+i));
    }
    //Print MRB/MRF
    printk("MR0F:%08x\n\r",*(exp_addr+0x7f));
    printk("MR1F:%08x\n\r",*(exp_addr+0x80));
    printk("MR2F:%08x\n\r",*(exp_addr+0x81));
    printk("MR0B:%08x\n\r",*(exp_addr+0x82));
    printk("MR1B:%08x\n\r",*(exp_addr+0x83));
    printk("MR2B:%08x\n\r",*(exp_addr+0x84));
    return true;
    }

}


// =============================================================================
// �������ܣ�_ExpSpecialDecoder
//           �˲������������쳣��Ϣ�з�ͨ����Ϣ����Բ�ͬ�쳣���ͣ������������Ϣ��
// ���������u32 major��    ���쳣���
//           u32 exp_len,   �쳣�洢��Ϣ����(SHARC21469 1���ֽ�=32bit)
// �����������
// ����ֵ  ���ɹ���������true,���󷵻�false��
// =============================================================================

bool_t __Exp_SpecialDecoder(u32 *exp_addr,u32 major,u32 exp_len)
{
    u32 i=0;
     switch(major)
    {
        case  2||5||6:
        __Exp_InstructionDecoder(exp_addr);
          break;
        case  3:
        __Exp_HardStackDecoder(exp_addr, exp_len);
          break;
        case  4:
        __Exp_SportDecoder(exp_addr,exp_len);
         break;
        case 7:
        __Exp_InstructionDecoder(exp_addr);
        break;
        case 8:
        __Exp_InstructionDecoder(exp_addr);
        break;
        case 9:
        __Exp_InstructionDecoder(exp_addr);
        break;
        case 10:
        __Exp_InstructionDecoder(exp_addr);
        break;
        default:
        break;
    }



}


// =============================================================================
// �������ܣ�__SwapExpCpuInfoByEndian
//          ת��Ӳ��CPU�쳣��Ϣ�ֽ���
// ���������cpuinfo, CPU�쳣ͨ����Ϣ
// ���������cpuinfo, CPU�쳣ͨ����Ϣ��ת����
// ����ֵ  ����
// ˵��    ���ڲ�����
// =============================================================================
void __Exp_SwapCpuInfoByEndian(u32 *pAddr,u32 exp_len)
{
    //u32 *temp;
    u32 i=0;
    for(i = 0; i<exp_len; i++)
    {
        *pAddr = swapl(*pAddr);
        pAddr++;
    }
}


// =============================================================================
// �������ܣ�exception_harddecoder
//           �˲�����Ҫ��������������ͨ���쳣��Ϣ
// ���������parahead���׳�ʱ�Ĳ���
//           endian, ��Ϣ�Ĵ洢��ʽ
// �����������
// ����ֵ  : true�ɹ�, falseʧ��
// ˵��    ��
// =============================================================================
bool_t  Exp_Harddecoder(struct ExpThrowPara *parahead, u32 endian)
{
    bool_t result = false;
    u32 exp_len=0;
    u32 major=0;
    u32 * exp_addr;

    //struct SysExpCpuInfo     *CpuInfo;

    if(parahead == NULL)
    {
        //�Ǳ�����Ϣ���������
        printk("cpuinfodecoder:para error or no need to decode-- SHARC21469 decoder\n\r");
        result = false;
    }
    else
    {
            //��������������������������
            //CpuInfo = (struct SysExpCpuInfo *)(parahead->para);
            exp_addr=(u32 *)(parahead->ExpInfo);
            exp_len=parahead->ExpInfoLen;

            //�洢��ת��
            if(endian != CN_CFG_BYTE_ORDER)
            {
                __Exp_SwapCpuInfoByEndian(exp_addr,exp_len);
            }
            if(cn_sysexp_cpuinfo_validflag == *exp_addr)//��ǰ�汾�����Խ���
            {
                printk("CPUINFO LIST:\n\r");
                printk("cpuinfo:len = 0x%08x\n\r",parahead->para_len);
                printk("cpuinfo:major =0x%08x, %s\n\r",*(exp_addr+1),\
                        s_pcMajorExpNameTab[(*(exp_addr+1))]);
                printk("cpuinfo:minor =0x%08x, %s\n\r",*(exp_addr+2),\
                        s_pcMinorExpNameTab[(*(exp_addr+2))]);
                printk("cpuinfo:deal result = 0x%08x\n\r", parahead->ExpAction);
                //ͨ����Ϣ����
                exp_addr=exp_addr+5;
                exp_len=CN_SYSEXP_UNIVERSAL_LEN;

                __Exp_UniversalDecoder(exp_addr,exp_len);

                //������Ϣ����

                major=*(exp_addr-4);
                exp_len=parahead->para_len-0x5-CN_SYSEXP_UNIVERSAL_LEN;
                exp_addr=(u32 *)((parahead->para)+0x5+CN_SYSEXP_UNIVERSAL_LEN);
                __Exp_SpecialDecoder(exp_addr, major,exp_len);
                result = true;


        }
        else
        {
            printk("cpuinfodecoder:incomplete info frame!\n\r");
            result = false;
        }
    }

    return result;
}
//------------------------------------------------------------------------------
//function : this is used to set the exception vectors.
//parameter: NULL, we will set the vector address by the symbols in the asm file
//return   : NULL, just set the exception tab, only could execute once
//remarks  : sensitive to transplant,we do it the asm file
//           if you want to debug the code, please set the vector in the initial
//           stage which means set in the asm mode, else please set it in the
//           C mode which means in this function.
//------------------------------------------------------------------------------

bool_t HardExp_InfoDecoderInit(void)
{
    static struct ExpInfoDecoder HardExpDecoder;
    bool_t result;
    HardExpDecoder.MyDecoder = HardExp_Decoder;
    HardExpDecoder.DecoderName = CN_HARDEXP_INFODECODERNAME;
    result = Exp_RegisterThrowInfoDecoder(&HardExpDecoder);
    __asm_exp_init();

    return true;
}



ptu32_t module_exp_shell_init(ptu32_t para)
{
    para=para;
    return 1;
}





//------------------------------------------------------------------------------
//function:this function is used to erase exception information in the flash.
//parameter:
//return:
//remarks:sensitive to transplant
//------------------------------------------------------------------------------
bool_t Sh_SoftwareReset(char *param)
{
    __asm_software_reset();
    return true;
}


//------------------------------------------------------------------------------
//function:this function is used to CPU_Reboot the os .
//parameter:
//return:
//remarks:sensitive to transplant
//------------------------------------------------------------------------------

extern void ___second_stage_reboot(u32);

bool_t Sh_Reboot(char *param)
{
  u32 flag;
   char *word_addr,*next_param;
    //��ȡ����
    word_addr = shell_inputs(param,&next_param);
    if(word_addr == NULL)
    {
        printf("\r\n��ʽ������ȷ��ʽ�ǣ�\r\n�쳣��Ŀ\r\n");
        return false;
    }
    flag = strtol(word_addr, (char **)NULL, 0);
    if(flag==monitor_reboot)
        __second_stage_reboot(flag);
    else if(flag==release_reboot)
        __second_stage_reboot(flag);
    else
        return false;


 return true;
}

//------------------------------------------------------------------------------
//function:this function is used to restart the operation syatem .
//parameter:
//return:
//remarks:sensitive to transplant
//------------------------------------------------------------------------------
bool_t Sh_RestartSystem(char * param)
{
    u32 flag;
#ifdef Release_version
    flag=release_reboot;
#endif
#ifdef Monitor_version
    flag=monitor_reboot;
#endif
    __second_stage_reboot(flag);

return true;
}

// =============================================================================
// �������ܣ�CPU_Reset
//           ��λCPU
// �����������
//
// �����������
// ����ֵ  : ��
// ˵��    ��
// =============================================================================
void CPU_Reset(void)
{
    __asm_software_reset();
}


// =============================================================================
// �������ܣ�CPU_Reboot
//           CPU_Reboot CPU
// �����������
//
// �����������
// ����ֵ  : ��
// ˵��    ��
// =============================================================================

void CPU_Reboot(void)
{

}

// =============================================================================
// �������ܣ�CPU_RestartSystem
//           CPU_Reboot CPU
// �����������
//
// �����������
// ����ֵ  : ��
// ˵��    ��
// =============================================================================

void CPU_RestartSystem(void)
{
}














