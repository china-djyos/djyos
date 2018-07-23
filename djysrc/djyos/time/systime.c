//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
//����ģ��:ʱ��ģ��
//����:  ������
//�汾��V1.0.0
//�ļ�����: Ϊϵͳ�ṩʱ������
//����˵��:
//�޶���ʷ:
// 1. ����: 2012-5-1
//   ����:  ������
//   �°汾��: V1.1.0
//   �޸�˵��: ������޸�
// 1. ����: 2011-10-28
//   ����:  ������
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "atomic.h"
#include "int.h"
#include "systime.h"
#include "board-config.h"
#include "component_config_time.h"

extern u64 __DjyGetSysTime(void);
extern u32 __Djy_GetTimeBaseCnt(void);
extern u32 __Djy_GetTimeBaseReload(void);
static fntSysTimeHard32  fnSysTimeHard32 = NULL;
static fntSysTimeHard64  fnSysTimeHard64 = NULL;
static u32 s_u32SysTimeFreq = 1000000;  //ϵͳʱ�ӵ�����ʱ��Ƶ��
                                        //1000000��tickʱ����Ĭ��ֵ
static u32 s_u32SysTimeCycle = CN_CFG_TICK_US;  //ϵͳʱ����ʱ���ڣ�CN_CFG_TICK_US
                                                //��ѡtickʱ��Ĭ��ֵ
static s64 s_s64sysTimeMajor = 0;       //ʱ�Ӹ�λ
static u32 s_u32BakCounter = 0;         //����ֵ�ı���

// =============================================================================
// �������ܣ�����ר�ö�ʱ����ϵͳʱ��
// ���������GetSysTime32�����Ӳ����ʱ��ʱ��32λ�����£������NULL
//           GetSysTime64�����Ӳ����ʱ��ʱ��64λ�������NULL
//           Freq����ʱ������ʱ�ӵ�Ƶ��
//           Cycle����ʱ����ת���ڣ�ʱ������64λӲ����ʱ����0����
// ���������
// ����ֵ  ��
// ˵�����ú��������ϵͳ��ʼ����ʱ����ã���ز�����ϵͳ�������ٵ���
// =============================================================================
void SysTimeConnect(fntSysTimeHard32 GetSysTime32,fntSysTimeHard64 GetSysTime64,
                    u32 Freq,u32 Cycle)
{
    //��Ҫ��SysTime�Ƿ�ΪNULL��NULLҲ��һ����ȷѡ�
    fnSysTimeHard32 = GetSysTime32;
    fnSysTimeHard64 = GetSysTime64;
    s_u32SysTimeFreq = Freq;
    s_u32SysTimeCycle = Cycle;
    return;
}
// =============================================================================
// ��������:��ȡ64λ��ϵͳ����ʱ�䣬��λus
// �����������
// ���������
// ����ֵ  ����ȡ��ϵͳ����ʱ�䣬uS
// =============================================================================
u64 DjyGetSysTime(void)
{
    u32 CurrentTime;
    s64 s64sysTimeMajorBak;
    atom_low_t atom;

    if((fnSysTimeHard32 == NULL) && (fnSysTimeHard64 == NULL))
        return __DjyGetSysTime();
    else if(fnSysTimeHard32 != NULL)
    {
        atom = Int_LowAtomStart();
        CurrentTime = fnSysTimeHard32( );
        if(CurrentTime < s_u32BakCounter)
            s_s64sysTimeMajor += s_u32SysTimeCycle; //�˴�����Ҫԭ�Ӳ���
        s_u32BakCounter = CurrentTime;
        s64sysTimeMajorBak = s_s64sysTimeMajor;
        Int_LowAtomEnd(atom);
        //�Ӽ���ֵ����uS��
        return (s64sysTimeMajorBak+CurrentTime)*1000000/s_u32SysTimeFreq;
    }
    else
    {
        return fnSysTimeHard64( ) *1000000 / s_u32SysTimeFreq;
    }
}

// =============================================================================
// ��������:��ȡ64λ��ϵͳ����ʱ�䣬��λcycle������DjyGetSysTimeFreq����ȷ��
//          ����ʱ��
// �����������
// ���������
// ����ֵ  ����ȡ��ϵͳ����ʱ�䣬������
// =============================================================================
u64 DjyGetSysTimeCycle(void)
{
    u32 CurrentTime;
    u64 s64sysTimeMajorBak;
    atom_low_t atom;

    if((fnSysTimeHard32 == NULL) && (fnSysTimeHard64 == NULL))
        return __DjyGetSysTime();
    else if(fnSysTimeHard32 != NULL)
    {
        atom = Int_LowAtomStart();
        CurrentTime = fnSysTimeHard32( );
        if(CurrentTime < s_u32BakCounter)
            s_s64sysTimeMajor += s_u32SysTimeCycle; //�˴�����Ҫԭ�Ӳ���
        s_u32BakCounter = CurrentTime;
        s64sysTimeMajorBak = s_s64sysTimeMajor;
        Int_LowAtomEnd(atom);
        return s64sysTimeMajorBak+CurrentTime;;
    }
    else
    {
        return fnSysTimeHard64( );
    }
}

// =============================================================================
// ��������:��ʱ��ά������tick isr�е��ã�ȷ����ʹ�÷�64λ��ʱ����ʱ��ĺ�������
//          �����ã���ά��ϵͳʱ����ת��
// �����������
// ���������
// ����ֵ  ����ȡ��ϵͳ����ʱ�䣬������
// =============================================================================
void __DjyMaintainSysTime(void)
{
    u32 CurrentTime;
    if(fnSysTimeHard32 != NULL)
    {
        CurrentTime = fnSysTimeHard32( );
        if(CurrentTime < s_u32BakCounter)
            s_s64sysTimeMajor += s_u32SysTimeCycle; //�˴�����Ҫԭ�Ӳ���
        s_u32BakCounter = CurrentTime;
    }
    return;
}

// =============================================================================
// ��������:��ȡϵͳʱ�ӵļ�ʱƵ��
// �����������
// ���������
// ����ֵ  ��ʱ��Ƶ�ʣ������ticks����̶�����1Mhz
// =============================================================================
u32 DjyGetSysTimeFreq(void)
{
    return s_u32SysTimeFreq;
}
//----��ȡ��ǰticks-------------------------------------------------------------
//���ܣ���ȡ����ϵͳʱ��ticks��
//      g_s64OsTicks Ϊ64λ��������64λϵͳ�У���ȡ g_s64OsTicks ��Ҫ����1��
//      ����,��Ҫʹ��ԭ�Ӳ�����
//��������
//���أ���ǰʱ��
//-----------------------------------------------------------------------------
u64 DjyGetSysTimeBase(void)
{
	return __DjyGetSysTime();
}

