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
// ģ������:hard-exp.h/p1020-core0
// ģ��汾:
// ������Ա: Administrator
// ����ʱ��: 5:28:05 PM/Sep 16, 2013
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================

#ifndef EXCEPTION_H_
#define EXCEPTION_H_
#ifdef __cplusplus
#extern "C" {
#endif

#define monitor_reboot  1
#define release_reboot  2

//reserved 256K (block 27~block 30)for the exception record.20K for the tagSysExpRecordItem.
//#define cn_sys_exp_record_size                     0x40000     //256K for the exception record.
#define cn_sys_exp_record_size_basic               0x10000      //20K for the basic exp information(struct SysExpRecordItem).
#define cn_sys_exp_record_infor_basic_offset_addr  0x10000   //the start address for save the exp information.
#define cn_exp_tab_used_flag              0x00
//Flash�д洢exp����ַ
#define cn_sys_exp_record_max_address     0x44FFFFF
#define cn_sys_exp_sector_base_number     0x32
#define cn_sys_exp_sector_max_number      0x39
#define sector_max_offset_addr            0x1FFFF
#define CN_MAJOREXP_INVALID  0xa
#define CN_MINOREXP_INVALID 0x28



#define Exception_StartAddress 0x000E6E05

#define sys_cmode_dealer_result_succeed 0
#define sys_cmode_dealer_result_asmdo   1
#define sys_cmode_dealer_result_restart 2


//declare the function for exception
//ASM
#define  __asm_reset_cpu_hot  __asm_software_reset




//SPERRSTAT ��SPERRCTLx�Ĵ���bit ����
#define BIT_0   0x00000001
#define BIT_1   0x00000002
#define BIT_2   0x00000004
#define BIT_3   0x00000008
#define BIT_4   0x00000010
#define BIT_5   0x00000020
#define BIT_6   0x00000040
#define BIT_7   0x00000080
#define BIT_8   0x00000100
#define BIT_9   0x00000200
#define BIT_10  0x00000400
#define BIT_11  0x00000800
#define BIT_12  0x00001000
#define BIT_13  0x00002000
#define BIT_14  0x00004000
#define BIT_15  0x00008000
#define BIT_16  0x00010000
#define BIT_17  0x00020000
#define BIT_18  0x00040000
#define BIT_19  0x00080000
#define BIT_20  0x00100000
#define BIT_21  0x00200000
#define BIT_22  0x00400000
#define BIT_23  0x00800000
#define BIT_24  0x01000000
#define BIT_25  0x02000000
#define BIT_26  0x04000000
#define BIT_27  0x08000000
#define BIT_28  0x10000000
#define BIT_29  0x20000000
#define BIT_30  0x40000000
#define BIT_31  0x80000000

//����SPERRSTAT �Ĵ���λ
#define SP0_DERRA (BIT_0)
#define SP0_DERRB (BIT_1)
#define SP1_DERRA (BIT_2)
#define SP1_DERRB (BIT_3)
#define SP2_DERRA (BIT_4)
#define SP2_DERRB (BIT_5)
#define SP3_DERRA (BIT_6)
#define SP3_DERRB (BIT_7)
#define SP4_DERRA (BIT_8)
#define SP4_DERRB (BIT_9)
#define SP5_DERRA (BIT_10)
#define SP5_DERRB (BIT_11)
#define SP6_DERRA (BIT_12)
#define SP6_DERRB (BIT_13)
#define SP7_DERRA (BIT_14)
#define SP7_DERRB (BIT_15)
#define SP0_FSERR (BIT_16)
#define SP1_FSERR (BIT_17)
#define SP2_FSERR (BIT_18)
#define SP3_FSERR (BIT_19)
#define SP4_FSERR (BIT_20)
#define SP5_FSERR (BIT_21)
#define SP6_FSERR (BIT_22)
#define SP7_FSERR (BIT_23)

//����SPERRCTLx�Ĵ���λ
#define DERRA_STAT (BIT_4)
#define DERRB_STAT (BIT_5)
#define FSERR_STAT (BIT_6)




//���һЩCPU�ļĴ�����Ϣ������Ϣ�����ڸþ���CPU�ͺŵģ�CORE��Ϣ+ CPU������Ϣ��
//ϵͳӲ�쳣--CPUʵ���쳣
 /*struct SysExpCpuInfo
{
    u32    dwSysExpCpuFlag;      //���쳣��Ϣ����Ч����
    u32    dwExpMajor;            //�쳣�����
    u32    dwExpMinor;            //�쳣��֧���
    u32    dwExpAddr;             //�쳣���ڴ��д洢�ĵ�ַ
    u32    dwExpLen;              //�����쳣�洢����Ϣ�ĳ���
};*/

#define cn_sysexp_cpuinfo_validflag     ((u32)(0x87654321))


#ifdef __cplusplus
}
#endif
#endif /* EXCEPTION_H_ */
