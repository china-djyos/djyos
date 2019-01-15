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
//����ģ��:�ж�ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: cm3�쳣����ģ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-08-30
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��:
//------------------------------------------------------
#include "stdint.h"
#include "hard-exp.h"
#include "blackbox.h"
#include "stdio.h"
#include "endian.h"
#include "cpu_peri.h"
#include "int.h"
#include "djyos.h"
#include "dbug.h"
bool_t  HardExp_Decoder(struct BlackBoxThrowPara *parahead, u32 endian);

// =============================================================================
// �������ܣ��˲�����Ҫ��������������ͨ���쳣��Ϣ��NVIC�Ĳ��ִ����쳣��Ϣ�ļĴ���
// ���������layer,�׳����
//           parahead���׳�ʱ�Ĳ���
//           endian, ��Ϣ�Ĵ洢��ʽ
// �����������
// ����ֵ  : true�ɹ�, falseʧ��
// =============================================================================
bool_t  HardExp_Decoder(struct BlackBoxThrowPara *parahead, u32 endian)
{
    bool_t result = false;
    struct SysExceptionInfo  *mycpuinfo;

    if(parahead == NULL)
    {
        //�Ǳ�����Ϣ���������
        printk("CPU����Ӳ��û���쳣\r\n");
        result = false;
    }
    else
    {
        if((sizeof(struct SysExceptionInfo))== parahead->BlackBoxInfoLen)//�������Ƿ�����
        {   /*
            //��������������������������
            mycpuinfo = (struct SysExceptionInfo *)(parahead->para);
            //�洢��ת��
            if(endian != CN_CFG_BYTE_ORDER)
            {
                __SwapExpCpuInfoByEndian(mycpuinfo);
            }
            if(CN_SYS_EXP_CPUINFO_VALIDFLAG == mycpuinfo->SysExpCpuFlag)//��ǰ�汾�����Խ���
            {
                printk("CPUINFO LIST:\r\n");
                printk("CPUINFO:len = 0x%08x\n\r",parahead->para_len);
                printk("CPUINFO:deal result = 0x%08x\n\r", parahead->ExpAction);

                //EXP FAULT ��Ϣ����
                printk("SysExpInfo:Cortex M3/M4 General Registers:\n\r");
                printk("R0:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r0);
                printk("R1:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r1);
                printk("R2:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r2);
                printk("R3:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r3);
                printk("R4:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r4);
                printk("R5:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r5);
                printk("R6:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r6);
                printk("R7:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r7);
                printk("R8:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r8);
                printk("R9:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r9);
                printk("R10:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r10);
                printk("R11:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r11);
                printk("R12:0x%08x\n\r",mycpuinfo->CoreInfo.ger_r12);

                printk("SysExpInfo:Cortex M3/M4 Special Registers:\n\r");
                printk("SP:0x%08x\n\r",mycpuinfo->CoreInfo.spr_sp);
                printk("LR:0x%08x\n\r",mycpuinfo->CoreInfo.spr_lr);
                printk("PC:0x%08x\n\r",mycpuinfo->CoreInfo.spr_pc);
                printk("XPSR:0x%08x\n\r",mycpuinfo->CoreInfo.spr_xpsr);

                printk("SysExpInfo:NVIC Exception Registers:\n\r");
                printk("MFSR:0x%02x\n\r",mycpuinfo->NvicInfo.mfsr);
                printk("BFSR:0x%02x\n\r",mycpuinfo->NvicInfo.bfsr);
                printk("UFSR:0x%04x\n\r",mycpuinfo->NvicInfo.ufsr);
                printk("HFSR:0x%08x\n\r",mycpuinfo->NvicInfo.hfsr);
                printk("DFSR:0x%04x\n\r",mycpuinfo->NvicInfo.dfsr);

                result = true;
            }
            else
            {
                printk("CPU Info Decode:ivalid info flag!\n\r");
                result = false; //���ܲ���һ����CPU���߰汾���޷�����
            }
            */
        }
        else
        {
            printk("CPU Info Decode:incomplete info frame!\n\r");
            result = false;
        }
    }

    return result;
}

// =============================================================================
// ����: ע��Ӳ���쳣�����ݽ�������, ����ƽ̨�����ṩ�������,���쳣ģ�����,����
//       �������쳣ģ����벻ͨ��.
// ����: ��
// ����ֵ     :true�ɹ�, falseʧ��
// =============================================================================
bool_t HardExp_InfoDecoderInit(void)
{
    static struct BlackBoxInfoDecoder HardExpDecoder;
    bool_t result;
    HardExpDecoder.MyDecoder = HardExp_Decoder;
    HardExpDecoder.DecoderName = CN_HARDEXP_DECODERNAME;
    result = BlackBox_RegisterThrowInfoDecoder(&HardExpDecoder);

    return result;
}

// =============================================================================
// �������ܣ��쳣��ʼ��,�����쳣������ĳ�ʼ��
// �����������ʱ�ޣ�����������
// �����������
// ����ֵ  : true�ɹ�, falseʧ��
// =============================================================================
void HardExp_Init(void)
{
    //__Exp_TableSet();
}
