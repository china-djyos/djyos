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
#include "cpu.h"
#include "systime.h"
#include "component_config_time.h"

extern s64 __DjyGetSysTime(void);
extern s64  g_s64OsTicks;               //����ϵͳ����ticks��
static fnSysTimeHard32  s_fnSysTimeHard32 = NULL;
static fnSysTimeHard64  s_fnSysTimeHard64 = NULL;
static u32 s_u32SysTimeFreq = 1000000;  //ϵͳʱ�ӵ�����ʱ��Ƶ��
                                        //1000000��tickʱ����Ĭ��ֵ
static u32 s_u32SysTimeCycle = CN_CFG_TICK_US;  //ϵͳʱ����ʱ���ڣ�CN_CFG_TICK_US
                                                //��ѡtickʱ��Ĭ��ֵ
static s64 s_s64sysTimeMajor = 0;       //ʱ�Ӹ�λ
static u32 s_u32BakCounter = 0;         //����ֵ�ı���

//__attribute__((weak)) s64 __DjyGetSysTime(void)
//{
//    debug_printf("systime", "��bsp�ṩ __DjyGetSysTime����")
//    return 0;
//}

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
void DJY_SysTimeConnect(fnSysTimeHard32 GetSysTime32,fnSysTimeHard64 GetSysTime64,
                    u32 Freq,u32 Cycle)
{
    //��Ҫ��GetSysTime32��GetSysTime64�Ƿ�ΪNULL��NULLҲ��һ����ȷѡ�
    s_fnSysTimeHard32 = GetSysTime32;
    s_fnSysTimeHard64 = GetSysTime64;
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
s64 DJY_GetSysTime(void)
{
    u32 CurrentTime;
    s64 s64sysTimeMajorBak;
    atom_low_t atom;

    if((s_fnSysTimeHard32 == NULL) && (s_fnSysTimeHard64 == NULL))
        return __DjyGetSysTime();
    else if(s_fnSysTimeHard32 != NULL)
    {
        atom = Int_LowAtomStart();
        CurrentTime = s_fnSysTimeHard32( );
        if(CurrentTime < s_u32BakCounter)
            s_s64sysTimeMajor += s_u32SysTimeCycle; //�˴�����Ҫԭ�Ӳ���
        s_u32BakCounter = CurrentTime;
        s64sysTimeMajorBak = s_s64sysTimeMajor;
        Int_LowAtomEnd(atom);
        //�Ӽ���ֵ����uS��
        //return (s64sysTimeMajorBak+CurrentTime)*1000000/s_u32SysTimeFreq;
        return ((s64)((s64sysTimeMajorBak+CurrentTime)/ s_u32SysTimeFreq) *1000000 +
                (((s64)((s64sysTimeMajorBak+CurrentTime)% s_u32SysTimeFreq) *1000000)/ s_u32SysTimeFreq));
    }
    else
    {
        //return s_fnSysTimeHard64( ) *1000000 / s_u32SysTimeFreq;
        s64 temp = (s64)s_fnSysTimeHard64( );
        return ((s64)(temp/ s_u32SysTimeFreq) *1000000 +
                        (((s64)(temp% s_u32SysTimeFreq) *1000000)/ s_u32SysTimeFreq));
    }
}

// =============================================================================
// ��������:��ȡ64λ��ϵͳ����ʱ�䣬��λcycle������DjyGetSysTimeFreq����ȷ��
//          ����ʱ��
// �����������
// ���������
// ����ֵ  ����ȡ��ϵͳ����ʱ�䣬������
// =============================================================================
s64 DJY_GetSysTimeCycle(void)
{
    u32 CurrentTime;
    s64 s64sysTimeMajorBak;
    atom_low_t atom;

    if((s_fnSysTimeHard32 == NULL) && (s_fnSysTimeHard64 == NULL))
        return __DjyGetSysTime();
    else if(s_fnSysTimeHard32 != NULL)
    {
        atom = Int_LowAtomStart();
        CurrentTime = s_fnSysTimeHard32( );
        if(CurrentTime < s_u32BakCounter)
            s_s64sysTimeMajor += s_u32SysTimeCycle; //�˴�����Ҫԭ�Ӳ���
        s_u32BakCounter = CurrentTime;
        s64sysTimeMajorBak = s_s64sysTimeMajor;
        Int_LowAtomEnd(atom);
        return s64sysTimeMajorBak+CurrentTime;;
    }
    else
    {
        return s_fnSysTimeHard64( );
    }
}

// =============================================================================
// ��������:��ʱ��ά������tick isr�е��ã�ȷ����ʹ�÷�64λ��ʱ����ʱ��ĺ�������
//          �����ã���ά��ϵͳʱ����ת��
// �����������
// ���������
// ����ֵ  ����ȡ��ϵͳ����ʱ�䣬������
// =============================================================================
void __DJY_MaintainSysTime(void)
{
    u32 CurrentTime;
    if(s_fnSysTimeHard32 != NULL)
    {
        CurrentTime = s_fnSysTimeHard32( );
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
u32 DJY_GetSysTimeFreq(void)
{
    return s_u32SysTimeFreq;
}
//----��ȡ��ǰticks-------------------------------------------------------------
//���ܣ���ȡ����ϵͳʱ��ticks����������һ��tickʱ���жϡ�
//      g_s64OsTicks Ϊ64λ��������64λϵͳ�У���ȡ g_s64OsTicks ��Ҫ����1��
//      ����,��Ҫʹ��ԭ�Ӳ�����
//��������
//���أ���ǰʱ��
//-----------------------------------------------------------------------------
s64 __DJY_GetSysTick(void)
{
    s64 time;
#if (64 > CN_CPU_BITS)
    //���������ֳ�����64λ,��Ҫ������ڲ��ܶ�ȡos_ticks,�ù��̲��ܱ�ʱ���жϴ��.
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart();
#endif

    time = g_s64OsTicks;

#if (64 > CN_CPU_BITS)
    //���������ֳ�����64λ,��Ҫ������ڲ��ܶ�ȡos_ticks,�ù��̲��ܱ�ʱ���жϴ��.
    Int_LowAtomEnd(atom_low);
#endif
    return time;
}

