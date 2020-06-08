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
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: �ж�ģ����Ӳ����صĴ��룬�����첽�ź���ʵʱ�ж�
//����˵��:
//�޶���ʷ:
//1. ����: 2010-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ��ԭint.c�а�Ӳ���޹ش����������
//------------------------------------------------------
#include "critical.h"
#include "stdint.h"
#include "stdlib.h"
#include "cpu_peri.h"
#include "cdef21469.h"
#include "def21469.h"
#include "sysreg.h"
#include "int.h"
#include "int_hard.h"
#include "critical.h"
#include "djyos.h"

extern struct IntMasterCtrl  tg_int_global;          //���岢��ʼ�����жϿ��ƽṹ
extern struct IntLine *tg_pIntLineTable[];       //�ж��߲��ұ�
extern void __DJY_ScheduleAsynSignal(void);
void __DJY_EventReady(struct EventECB *event_ready);

extern u32 __read_imask_seg(void);              // RETURN imask
extern u32 __read_lirptl_seg(void);         // RETURN lirptl
extern u32 __read_irptl_seg(void);              // RETURN irptl
extern void __set_imask_seg(u32 imask_seg);     // OR imask and imask_seg
extern void __set_lirptl_seg(u32 irptl_seg);    // OR lirptl and irptl_seg
extern void __set_irptl_seg(u32 irptl_seg);     // OR irptl and irptl_seg
extern void __clr_imask_seg(u32 imask_seg);     // AND imask and ~imask_seg
extern void __clr_lirptl_seg(u32 irptl_seg);    // AND lirptl and ~irptl_seg
extern void __clr_irptl_seg(u32 irptl_seg);     // AND irptl and ~irptl_seg

extern const ufast_t tg_IntUsedNum;      //��������int_config.c�ж���

void (*fg_vect_table[CN_INT_LINE_LAST+1])(void);

bool_t g_bIntAsynSignalEnable=false;    //�첽�ź��ܿ���


//----��ͨ�첽�źſ���---------------------------------------------------------
//���ܣ���ͨ�첽�źſ���,����ܿ��ؽ�ͨ���ж��߿��ؽ�ͨ,���жϽ�������
//      1.���ж�����Ӳ������ʱ,�Ѹÿ��ؽ�ͨ����
//      2.���û�ж���Ӳ������,���ͨ���б�������첽�źŵ��߿���.
//      3.cm3���ڵ�һ�����,��BASEPRI�Ĵ�����Ϊ0������ȫ���첽�ź�
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_ContactAsynSignal(void)
{
    u32 enable_bitmap_low=0,enable_bitmap_high=0;//tg_int_global.enable_bitmap�ܹ�32*2λ,real=1,asyn=27
    u32 real_mask_temp=0;

    g_bIntAsynSignalEnable = true;

    enable_bitmap_high = tg_int_global.enable_bitmap[1];                //IICDI~FLTOI:32
    enable_bitmap_low = tg_int_global.enable_bitmap[0];                 //FLTUI~SFT3I:7

    real_mask_temp = (( enable_bitmap_low<<2 ) & ( 0x00000001<<4 ))     //TMZHI:1
                   | (( enable_bitmap_low<<3 ) & ( 0x000001fe<<8 ))     //IRQ1~P5I:8 (except IRQ2)
                   | (( enable_bitmap_low>>5 ) & ( 0x00000007<<17 ))    //P14I~P16I:3
                   | (( enable_bitmap_low>>7 ) & ( 0x00000001<<22 ))    //TMZLI:1
                   | (( enable_bitmap_high<<25 ) & ( (u32)0x0000000f<<28 ));    //SFT0I~SFT3I:4
    __set_imask_seg( real_mask_temp );      // OR imask and real_mask_temp
    real_mask_temp = (( enable_bitmap_low>>4 ) & ( 0x000000ff<<10 ))    //P6I~P13I:8
                   | (( enable_bitmap_low>>7 ) & ( 0x00000003<<18 ));   //P17I~P18I:2
    __set_lirptl_seg( real_mask_temp );     // OR lirptl and real_mask_temp
}

//----�Ͽ��첽�źſ���---------------------------------------------------------
//���ܣ��Ͽ��첽�źſ���,���е��첽�źŽ�����ֹ
//      1.���ж�����Ӳ������ʱ,�Ѹÿ��ضϿ�����
//      2.���û�ж���Ӳ������,��Ͽ������첽�źŵ��߿���.
//      3.cm3���ڵ�һ�����,��BASEPRI�Ĵ�����Ϊ�������ֹ�첽�ź�
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_CutAsynSignal(void)
{
    g_bIntAsynSignalEnable = false;

    sysreg_bit_clr(sysreg_IMASK, asyn_mask_imask );
    sysreg_bit_clr(sysreg_LIRPTL, asyn_mask_lirptl );
}
//----��ͨ���жϿ���-----------------------------------------------------------
//���ܣ���ͨ���жϿ���,����cpu������һ���ܿ���,ֱ�Ӳ����ÿ��ؼ���.
//SHARC�Ƚ����⣬�˴�ȡ����������ʹ�ܵ�real��asyn�źţ�����
//tg_int_global.enable_bitmap���жϣ����ɴﵽͬ��Ч��.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_ContactTrunk(void)
{
    u32 enable_bitmap_low=0,enable_bitmap_high=0;//tg_int_global.enable_bitmap�ܹ�32*2λ,real=1,asyn=27
    u32 real_mask_temp=0;

    g_bIntAsynSignalEnable = true;

    enable_bitmap_high = tg_int_global.enable_bitmap[1];                //IICDI~FLTOI:32
    enable_bitmap_low = tg_int_global.enable_bitmap[0];                 //FLTUI~SFT3I:7

    real_mask_temp = (( enable_bitmap_low<<2 ) & ( 0x00000001<<4 ))     //TMZHI:1
                   | (( enable_bitmap_low<<3 ) & ( 0x000001ff<<8 ))     //IRQ2~P5I:9 (include IRQ2)
                   | (( enable_bitmap_low>>5 ) & ( 0x00000007<<17 ))    //P14I~P16I:3
                   | (( enable_bitmap_low>>7 ) & ( 0x00000001<<22 ))    //TMZLI:1
                   | (( enable_bitmap_high<<25 ) & ( (u32)0x0000000f<<28 ));    //SFT0I~SFT3I:4
    __set_imask_seg( real_mask_temp );      // OR imask and real_mask_temp
    real_mask_temp = (( enable_bitmap_low>>4 ) & ( 0x000000ff<<10 ))    //P6I~P13I:8
                   | (( enable_bitmap_low>>7 ) & ( 0x00000003<<18 ));   //P17I~P18I:2
    __set_lirptl_seg( real_mask_temp );     // OR lirptl and real_mask_temp

}

//----�Ͽ����жϿ���---------------------------------------------------------
//���ܣ��Ͽ����жϿ���,����cpu������һ���ܿ���,ֱ�Ӳ����ÿ��ؼ���.
//SHARC�Ƚ����⣬�˴���������real��asyn�źż��ɴﵽͬ��Ч��.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_CutTrunk(void)
{
    g_bIntAsynSignalEnable = false;

    sysreg_bit_clr(sysreg_IMASK, real_mask_imask );
    sysreg_bit_clr(sysreg_LIRPTL, real_mask_lirptl );
}

//----����߼�ԭ�Ӳ���---------------------------------------------------------
//���ܣ�������ǰ���ж�״̬��Ȼ���ֹ���жϡ��߼�ԭ�Ӳ�����ָ�ڼ䲻�����κ�ԭ���ϵĲ�����
//     Int_HighAtomStart--int_high_atom_end��������ʹ�ã��ڱ�������ס�Ĵ�����ڲ�
//     ����������κ�ϵͳ���á�
// ��������
// ���أ�ԭ����ԭ��״̬
//��ע����ֲ����
//-----------------------------------------------------------------------------
//if we just use it in the exception mode,then we could the ee bits--TODO
atom_high_t Int_HighAtomStart(void)
{
    atom_high_t high;
    u32 ucl_imask,ucl_lirptl;

    ucl_imask = __read_imask_seg();
    ucl_lirptl = __read_lirptl_seg();

    high.imask = ucl_imask & real_mask_imask;       //ֻȡreal��asyn
    high.lirptl = ucl_lirptl & real_mask_lirptl;    //ֻȡreal��asyn

    sysreg_bit_clr(sysreg_IMASK, real_mask_imask );
    sysreg_bit_clr(sysreg_LIRPTL, real_mask_lirptl );

    return high;
}
//----�뿪�߼�ԭ�Ӳ���---------------------------------------------------------
//���ܣ��ָ������뿪ԭ�Ӳ���ǰ�����ж�״̬��������Ӧ��int_high_atom_start��������
//      Int_HighAtomStart--int_high_atom_end��������ʹ�ã��ڱ�������ס�Ĵ�����ڲ�
//     ����������κ�ϵͳ���á�
//������high��int_high_atom_start�����ԭ��״̬
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_HighAtomEnd(atom_high_t high)
{
    __set_imask_seg(high.imask);    //д��real��asyn
    __set_lirptl_seg(high.lirptl);  //д��real��asyn

}

//----����ͼ�ԭ�Ӳ���---------------------------------------------------------
//���ܣ���ȡ��ǰ�첽�źſ���״̬��Ȼ���ֹ�첽�źš��ͼ���ԭ�Ӳ�����������ֹ�ٽ�
//      ���ݵĲ������ʣ������ܻᱻʵʱ�жϴ�ϡ�
//      Int_LowAtomStart---int_low_atom_end��������ʹ�ã����Բ���Ƕ�ף��ڿ���
//      Ƕ�׵ĵط�������ʹ��int_save_asyn_signal���ر��ж�(����)��
//      ��������������"Int_RestoreAsynSignal--Int_SaveAsynSignal"�Ŀ��ٰ汾��
//      ��������Ƕ�׵����⣬�ڱ�������ס�Ĵ�����ڣ�Ҳ���������
//      int_restore_asyn_signal��int_save_asyn_signal�е��κ�һ����
//��������
//���أ�ԭ��״̬
//��ע����ֲ����
//-----------------------------------------------------------------------------
atom_low_t Int_LowAtomStart(void)
{
    atom_low_t low;
    u32 ucl_imask,ucl_lirptl;

    ucl_imask = __read_imask_seg();
    ucl_lirptl = __read_lirptl_seg();

    low.imask = ucl_imask & asyn_mask_imask;    //ֻȡasyn
    low.lirptl = ucl_lirptl & asyn_mask_lirptl; //ֻȡasyn

    sysreg_bit_clr(sysreg_IMASK, asyn_mask_imask );
    sysreg_bit_clr(sysreg_LIRPTL, asyn_mask_lirptl );

    return low;
}

//----�뿪�ͼ�ԭ�Ӳ���---------------------------------------------------------
//���ܣ��ָ�����ǰ�첽�ź����״̬��������Ӧ��int_low_atom_start�������ס�
//      Int_LowAtomStart---int_low_atom_end��������ʹ�ã����Բ���Ƕ�ף��ڿ���
//      Ƕ�׵ĵط�������ʹ��int_save_asyn_signal���ر��ж�(����)��
//      ��������������"Int_RestoreAsynSignal--Int_SaveAsynSignal"�Ŀ��ٰ汾��
//      ��������Ƕ�׵����⣬�ڱ�������ס�Ĵ�����ڣ�Ҳ���������
//      int_restore_asyn_signal��int_save_asyn_signal�е��κ�һ����
//������low��int_low_atom_start�����ԭ��״̬
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_LowAtomEnd(atom_low_t low)
{
    __set_imask_seg(low.imask);     //д��asyn
    __set_lirptl_seg(low.lirptl);   //д��asyn

}

//----��ͨ�����ж��߿���-------------------------------------------------------
//���ܣ���ͨ�����ж��߿���,���ж��Ƿ�����Ҫ������Ŀ���״̬
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
bool_t Int_ContactLine(ufast_t ufl_line)
{
    u32 ufl_line_h=0,ufl_line_l=0;
    u32 real_mask_temp=0;

    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_pIntLineTable[ufl_line] == NULL) )
        return false;

    if(g_bIntAsynSignalEnable==true)
    {
    if(ufl_line<32)
        ufl_line_l = 1 << ufl_line;
    else
        ufl_line_h = 1 << (ufl_line-32);

    real_mask_temp = (( ufl_line_l<<2 ) & ( 0x00000001<<4 ))        //TMZHI:1
                   | (( ufl_line_l<<3 ) & ( 0x000001fe<<8 ))        //IRQ1~P5I:8 (except IRQ2)
                   | (( ufl_line_l>>5 ) & ( 0x00000007<<17 ))       //P14I~P16I:3
                   | (( ufl_line_l>>7 ) & ( 0x00000001<<22 ))       //TMZLI:1
                   | (( ufl_line_h<<25 ) & ( (u32)0x0000000f<<28 ));        //SFT0I~SFT3I:4
    __set_imask_seg( real_mask_temp );      // OR imask and real_mask_temp
    real_mask_temp = (( ufl_line_l>>4 ) & ( 0x000000ff<<10 ))       //P6I~P13I:8
                   | (( ufl_line_l>>7 ) & ( 0x00000003<<18 ));      //P17I~P18I:2
    __set_lirptl_seg( real_mask_temp );     // OR lirptl and real_mask_temp
    }

    return true;
}

//----�Ͽ������ж��߿���-------------------------------------------------------
//���ܣ��Ͽ������ж��߿��أ��������жϺ��첽�źſ���״̬��Σ����ж��߱���ֹ
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
bool_t Int_CutLine(ufast_t ufl_line)
{
    u32 ufl_line_h=0,ufl_line_l=0;
    u32 real_mask_temp=0;

    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_pIntLineTable[ufl_line] == NULL) )
        return false;

    if(g_bIntAsynSignalEnable==true)
    {
    if(ufl_line<32)
        ufl_line_l = 1 << ufl_line;
    else
        ufl_line_h = 1 << (ufl_line-32);

    real_mask_temp = (( ufl_line_l<<2 ) & ( 0x00000001<<4 ))        //TMZHI:1
                   | (( ufl_line_l<<3 ) & ( 0x000001fe<<8 ))        //IRQ1~P5I:8 (except IRQ2)
                   | (( ufl_line_l>>5 ) & ( 0x00000007<<17 ))       //P14I~P16I:3
                   | (( ufl_line_l>>7 ) & ( 0x00000001<<22 ))       //TMZLI:1
                   | (( ufl_line_h<<25 ) & ( (u32)0x0000000f<<28 ));        //SFT0I~SFT3I:4
    __clr_imask_seg( real_mask_temp );      // AND imask and ~real_mask_temp
    real_mask_temp = (( ufl_line_l>>4 ) & ( 0x000000ff<<10 ))       //P6I~P13I:8
                   | (( ufl_line_l>>7 ) & ( 0x00000003<<18 ));      //P17I~P18I:2
    __clr_lirptl_seg( real_mask_temp );     // ADN lirptl and ~real_mask_temp
    }

    return true;
}

//----Ӧ���жϣ������Ӧ�ж��ߵ��жϹ���״̬-----------------------------------
//���ܣ�Ӳ��Ӧ������Ӧ�Ĺ��ܣ��ṩ����жϹ���Ĳ��������ǰ��������Ӧͬһ�ж���
//      �ĺ����жϣ�����󣬲ſ�����Ӧ�����жϡ�����������ж��߱�����Ϊʵʱ�ж�
//      �����첽�ź��޹�.
//      cm3�У���Ӧ�ж�ʱ�Զ������״̬�������������ֶ��������״̬
//������ufast ufl_line��ָ��Ӧ����ж��ߺ�
//���أ���
//��ע����Щ��ϵ�ж���ӦʱӲ��Ӧ�𣬱�����Ϊ�պ�����
//      ����������ֲ���к���
//ע�⣺SHARC���ж���ϵ�У�ֻ��DAI��DPI�ȼ����ж�����Ҫ���Ӧ��ģ�����Ϊ�Զ�Ӧ��
//DAI��DPI��Ӧ�������û������������
//-----------------------------------------------------------------------------
// SHARC do not need
bool_t Int_ClearLine(ufast_t ufl_line)
{
/*    ucpu_t  clear_temp;
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_pIntLineTable[ufl_line] == NULL) )
        return false;
    if(ufl_line == cn_int_line_DAII)
    {
        clear_temp = *pDAI_IRPTL_H;     //read to clear
        clear_temp = *pDAI_IRPTL_L;     //read to clear
    }
    else if(ufl_line == cn_int_line_DPII)
    {
        clear_temp = *pDPI_IRPTL;       //read to clear
    }
*/
        return true;
}

//----�����ж�-----------------------------------------------------------------
//����: ����һ���ж�.����жϱ�������,��������Ӱ��.����������ж��߱�����Ϊʵʱ
//      �жϻ����첽�ź��޹�
//������ufast ufl_line�����������ж��ߺ�
//���أ������Ӧ���ж���Ӳ�����ṩ����������жϹ���,���� false,���򷵻� true
//��ע: ������ʵ��������Ӳ��,��ЩӲ��ϵͳ��֧�ִ˹���.
//      ����������ֲ���к���
//-----------------------------------------------------------------------------
bool_t Int_TapLine(ufast_t ufl_line)
{
    u32 ufl_line_h=0,ufl_line_l=0;
    u32 real_mask_temp=0;

    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_pIntLineTable[ufl_line] == NULL) )
        return false;

    if(ufl_line<32)
        ufl_line_l = 1 << ufl_line;
    else
        ufl_line_h = 1 << (ufl_line-32);

    real_mask_temp = (( ufl_line_l<<2 ) & ( 0x00000001<<4 ))        //TMZHI:1
                   | (( ufl_line_l<<3 ) & ( 0x000001ff<<8 ))        //IRQ2~P5I:8 (include IRQ2)
                   | (( ufl_line_l>>5 ) & ( 0x00000007<<17 ))       //P14I~P16I:3
                   | (( ufl_line_l>>7 ) & ( 0x00000001<<22 ))       //TMZLI:1
                   | (( ufl_line_h<<25 ) & ( (u32)0x0000000f<<28 ));        //SFT0I~SFT3I:4
    __set_irptl_seg( real_mask_temp );      // OR irptl and real_mask_temp
    real_mask_temp = (( ufl_line_l>>14 ) & ( 0x000000ff ))          //P6I~P13I:8
                   | (( ufl_line_l>>17 ) & ( 0x00000003<<8 ));      //P17I~P18I:2
    __set_lirptl_seg( real_mask_temp );     // OR lirptl and real_mask_temp

    return true;
}

//----Ӧ��ȫ���жϣ����ȫ���ж��ߵ��жϹ���״̬-------------------------------
//���ܣ�Ӳ��Ӧ������Ӧ�Ĺ��ܣ��ṩ����жϹ���Ĳ���
//������ufast ufl_line��ָ��Ӧ����ж��ߺ�
//���أ���
//��ע����Щ��ϵ�ж���ӦʱӲ��Ӧ�𣬱�����Ϊ�պ�����
//      ����������ֲ���к���
//ע�⣺SHARC���ж���ϵ�У�ֻ��DAI��DPI�ȼ����ж�����Ҫ���Ӧ��ģ�����Ϊ�Զ�Ӧ��
//DAI��DPI��Ӧ�������û������������
//-----------------------------------------------------------------------------
// SHARC do not need
void __Int_ClearAllLine(void)
{
/*    ucpu_t  clear_temp;

    clear_temp = *pDAI_IRPTL_H;     //read to clear
    clear_temp = *pDAI_IRPTL_L;     //read to clear
    clear_temp = *pDPI_IRPTL;       //read to clear
*/
    return true;
}

//----��ѯ�ж�������״̬-------------------------------------------------------
//���ܣ���ѯ�������Ӧ�ж���״̬�������ڲ�ѯʽ�жϳ���
//������ufl_line������ѯ���ж���
//���أ����жϹ��𣬷���true�����򷵻�false
//��ע: ��Ӳ���ṹ���,��Щ�ṹ���ܲ��ṩ�������,����!
//      ����������ֲ���к���
//-----------------------------------------------------------------------------
bool_t Int_QueryLine(ufast_t ufl_line)
{
    u32 ufl_line_h=0,ufl_line_l=0;
    u32 ucl_irptl,ucl_lirptl;
    u32 real_mask_temp;

    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_pIntLineTable[ufl_line] == NULL) )
        return false;

    ucl_irptl = __read_irptl_seg();
    ucl_lirptl = __read_lirptl_seg();

    if(ufl_line<32)
        ufl_line_l = 1 << ufl_line;
    else
        ufl_line_h = 1 << (ufl_line-32);

    real_mask_temp = (( ufl_line_l<<2 ) & ( 0x00000001<<4 ))        //TMZHI:1
                   | (( ufl_line_l<<3 ) & ( 0x000001ff<<8 ))        //IRQ1~P5I:8 (include IRQ2)
                   | (( ufl_line_l>>5 ) & ( 0x00000007<<17 ))       //P14I~P16I:3
                   | (( ufl_line_l>>7 ) & ( 0x00000001<<22 ))       //TMZLI:1
                   | (( ufl_line_h<<25 ) & ( (u32)0x0000000f<<28 ));        //SFT0I~SFT3I:4
    if( (real_mask_temp & ucl_irptl)!=0 )
    {
        __clr_irptl_seg(real_mask_temp);
        return true;
    }
    else
    {
        real_mask_temp = (( ufl_line_l>>14 ) & ( 0x000000ff ))      //P6I~P13I:8
                        | (( ufl_line_l>>17 ) & ( 0x00000003<<8 )); //P17I~P18I:2
        if( (real_mask_temp & ucl_lirptl)!=0 )
        {
            __clr_lirptl_seg(real_mask_temp);
            return true;
        }
        else
        {
            return false;
        }
    }


}

//----��ָ���ж�������Ϊ�첽�ź�--------������---------------------------------
//���ܣ���ָ���ж�������Ϊ�첽�ź�,���ж�������Ӧ,��ǰ�жϷ��غ���Ч
//������ufast ufl_line��ָ�������õ��ж��ߺ�
//���أ�true=�ɹ���false=ʧ��
//      ��������ֲ����
//-----------------------------------------------------------------------------
// SHARC do not support
bool_t Int_SettoAsynSignal(ufast_t ufl_line)
{
    /*
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_pIntLineTable[ufl_line] == NULL) )
        return false;
    tg_pIntLineTable[ufl_line]->int_type = CN_ASYN_SIGNAL;       //�ж�������

    tg_int_global.property_bitmap[ufl_line/CN_CPU_BITS]
            &= ~(1<<(ufl_line % CN_CPU_BITS));              //����λͼ
    fg_vect_table[ufl_line] = __start_asyn_signal;   //������ָ���첽����
    pg_int_reg->pri[ufl_line] = 0xff;                       //�첽�ź����ȼ����
    */
    return true;
}

//----��ָ���ж�������Ϊʵʱ�ж�--------������---------------------------------
//���ܣ���ָ���ж�������Ϊʵʱ�ж�,���ж�������Ӧ,��ǰ�жϷ��غ���Ч
//������ufast ufl_line��ָ�������õ��ж��ߺ�
//���أ�true=�ɹ���false=ʧ��
//      ��������ֲ����
//-----------------------------------------------------------------------------
// SHARC do not support
bool_t Int_SettoReal(ufast_t ufl_line)
{
    /*
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_pIntLineTable[ufl_line] == NULL) )
        return false;
    if(tg_pIntLineTable[ufl_line]->sync_event != NULL)
        return false;     //���߳��ڵȴ�����жϣ�������Ϊʵʱ�ж�
    tg_pIntLineTable[ufl_line]->int_type = CN_REAL;    //�ж�������
    tg_int_global.property_bitmap[ufl_line/CN_CPU_BITS]
            |= 1<<(ufl_line % CN_CPU_BITS);   //����λͼ
    fg_vect_table[ufl_line] = __start_real;   //������ָ��ʵʱ�ж�����
    pg_int_reg->pri[ufl_line] = cn_real_prio_default; //��ΪĬ�����ȼ�
    */
    return true;
}

//----ʹ���ж�Ƕ��-------------------------------------------------------------
//����: ʹ��һ���ж�Դ����ռ��ʹ�ܺ���Ӧ���жϷ����ڼ䣬���ܻᱻ����ж���ռ��
//      ����������ʵʱ�жϻ����첽�źţ�������ʵ���йأ���ֲ��Ӧ�ø���Ӳ������
//      ��Ӧ������ȷ���Ƿ�����Ƕ�ס�
//      Ƕ�׷�����ʵʱ�ж�֮������첽�ź�֮�䡣ʵʱ�ж���Զ���Դ���첽�źţ���
//      ����Ƕ�ס�
//      ��������ϵ�е�ARMоƬ�жϹ��������ص㣬����ʵʱ�жϵĴ��������ַ�ʽ:
//      1����fiqʵ��ʵʱ�жϣ�INTOFFSET1��INTOFFSET2�Ĵ����ṩ�˵�ǰ���ڷ����
//         �жϺţ����������Ĵ�����fiq����Ч�����ʵʱ�ж���fiqʵ�֣�����Ҫ��λ
//         ��鱻��Ϊʵʱ�жϵ��жϺ���INTPND1��INTPND1�Ĵ����е�ֵ������ȷ����
//         ������жϺţ�������̿��ܷǳ���������������ܹ�ʵ��ʵʱ�ж�Ƕ�ף���
//         ȴ��ȫʧȥ��ʵʱ�жϵ���Ƴ��ԣ�djyos for s3c2416����ȡ���ַ�ʽ��
//      2����һ����������ǣ�ϵͳֻ��Ҫһ��ʵʱ�жϣ�����ȫ����Ϊ�첽�źţ�����
//         �������fiqʵ��ʵʱ�ж�����Ϊ����ģ���Ȼֻ��һ���жϣ�Ƕ��Ҳ���޴�
//         ̸���ˡ�
//      3��ʵʱ�жϺ��첽�źŶ�����irq��ʽʵ�֣�cpsr��Iλ����trunk���أ��첽�ź�
//         û�ж������أ���ͨ�������е������첽�źŵ��жϺŵĿ���ȫ���ص���ģ��
//         �첽�źſ��صġ���������ǲ���ʵ��ʵʱ�ж�Ƕ�׵ġ�Ϊʲô��?arm�����
//         Ϊ��ֻҪIλΪ0���µ��жϾͿ�����ռ���ڷ�����жϣ�Ҳ����ζ�ţ��첽��
//         ����ռʵʱ�жϣ���Ӳ����������ġ�ʵʱ�ж�Ҫʵ��Ƕ�ף����ȹص��첽��
//         �ţ�������I=0�����ʵʱ�ж���Ӧ�����첽�źű��ص�֮ǰ�����������첽
//         �źŷ����Ļ�����irq����ͻᷢ�������ص��첽�ź�Ҳû�ã����ж�һ����
//         �������첽�źžͻ���ռʵʱ�жϡ�������ռ��������"����"���ң�������
//         ���������⣬����ʹϵͳ�����������ռʵʱ�жϵ��첽�źţ������ڼ䷢��
//         ���߳��л�����fiq�����ջ������pg_event_running��ջ����ˣ�djyos for
//         2416�汾��֧��ʵʱ�ж�Ƕ�ס�
//         cortex-m3�У��첽�ź�ȫ������Ϊ������ȼ�������cm3���ж�Ƕ�׻��ƣ�������
//         ����ʵ��Ƕ�׵ġ�
//����: ufl_line�����������ж���
//����: ��
//-----------------------------------------------------------------------------
// SHARC do not support
bool_t Int_EnableNest(ufast_t ufl_line)
{
    /*
    if(tg_pIntLineTable[ufl_line]->int_type == CN_REAL)
    {
        tg_pIntLineTable[ufl_line]->enable_nest = true;
        return true;
    }
    else
    {
        return false;
    }*/
    return true;
}
//----��ֹ�ж�Ƕ��-------------------------------------------------------------
//����: ���ĳ�жϺŵ��ж�Ƕ��ʹ�ܡ�
//����: ufl_line�����������ж���
//����: ��
//-----------------------------------------------------------------------------
// SHARC do not support
bool_t Int_DisableNest(ufast_t ufl_line)
{
    /*
    if(tg_pIntLineTable[ufl_line]->int_type == CN_REAL)
    {
        tg_pIntLineTable[ufl_line]->enable_nest = false;
        return true;
    }
    else
    {
        return false;
    }*/
    return true;
}

//----�趨��ռ���ȼ�-----------------------------------------------------------
//����: �趨ָ���ж��ߵ�Ƕ�����ȼ�����������������Ӳ�����ܡ����Ӳ����֧�֣��ɷ���false
//������ufast ufl_line��ָ�������õ��ж��ߺ�
//���أ���
//ע: ��������ֲ����
//-----------------------------------------------------------------------------
// SHARC do not support
bool_t Int_SetPrio(ufast_t ufl_line,u32 prior)
{
    return true;
}

//----��ʼ���ж�---------------------------------------------------------------
//���ܣ���ʼ���ж�Ӳ��,��ʼ���ж������ݽṹ
//      2.�첽�źű��ֽ�ֹ,�������߳����������д�.
//      3.���ж�����
//      �û���ʼ������Ӧ���������¹���:
//      1.ϵͳ��ʼʱ���Ѿ���ֹ�����첽�ź�,�û���ʼ��ʱ���뵣���첽�źŷ���.
//      2.��ʼ�������������Ҫ�������ж�/ʵʱ�ж�/�첽�ź�,Ӧ�óɶ�ʹ��.��ֹʹ
//        �첽�ź�ʵ�ʴ�������״̬(���첽�����жϿ���ͬʱ����).
//      3.���Բ����ж���,�������ӡ�������ֹ��,��Ӧ�óɶ�ʹ��.
//      4.����ʹ��save/restore������,��Ҫʹ��enable/disable������.
//��������
//���أ���
//-----------------------------------------------------------------------------
//int the interrupt,after this you can use the interrupt
void Int_Init(void)
{
    ufast_t ufl_line;
    u32 para;

    Int_CutTrunk();//close the main int key
    __Int_ClearAllLine();//compatible to the old
////    pg_scb_reg->CCR |= 1<<bo_scb_ccr_usersetmpend;
    for(ufl_line=0;ufl_line <= CN_INT_LINE_LAST;ufl_line++)
    {
        tg_pIntLineTable[ufl_line] = NULL;
//        fg_vect_table[ufl_line] = __start_asyn_signal;  //ȫ����ʼ��Ϊ�첽�ź�
    }

    //SHARCĬ�ϣ�����IRQ2��Ϊʵʱ�ź�.
    tg_pIntLineTable[cn_int_line_IRQ2I]->int_type = CN_REAL;
   tg_pIntLineTable[cn_int_line_IRQ1I]->int_type = CN_REAL;
   tg_pIntLineTable[cn_int_line_IRQ0I]->int_type = CN_REAL;

    //SHARCĬ�ϣ�����DAI��clear_typeΪ����ISR�󣬷����޷�����ISR.
    tg_pIntLineTable[cn_int_line_DAII]->clear_type = CN_INT_CLEAR_POST;
    //SHARCĬ�ϣ�����DPI��clear_typeΪ����ISR�󣬷����޷�����ISR.
    tg_pIntLineTable[cn_int_line_DPII]->clear_type = CN_INT_CLEAR_POST;

    for(ufl_line=0; ufl_line < CN_INT_BITS_WORDS; ufl_line++)
    {
        //����λͼ����,ȫ����Ϊ�첽�źŷ�ʽ
        tg_int_global.property_bitmap[ufl_line] = 0;
        //�ж�ʹ��λͼ��0,ȫ�����ڽ�ֹ״̬
        tg_int_global.enable_bitmap[ufl_line] = 0;
    }
    tg_int_global.nest_asyn_signal =0;
    tg_int_global.nest_real=0;

//    tg_int_global.en_asyn_signal = false;
    tg_int_global.en_asyn_signal_counter = 1;   //�첽�źż���
    Int_CutAsynSignal();

//    tg_int_global.en_trunk = true;
    tg_int_global.en_trunk_counter = 0;       //���жϼ���
    Int_ContactTrunk();                       //��ͨ���жϿ���
}


//----���ж�����---------------------------------------------------------------
//���ܣ���Щϵͳ�����ж�����������������ʵʱ�жϻ����첽�źŵģ����߲�ϣ���ڻ�
//      ��׶�ʹ�ù��ڸ��ӵĴ���ģ�����2416��2440�ȣ����ڻ��׶λ�ȡ�жϺź�
//      ֱ�ӵ��ñ��������ɱ���������������첽�ź������ʵʱ�ж����档
//      ��cortex-m3��omapl138���������ж��������־Ϳ�������ʵʱ�жϻ����첽��
//      �ţ��������ṩ�������������ڻ�ಿ��ֱ�ӵ�����Ӧ�����档
//������ufast ufl_line����Ӧ���ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------
/*
void __Int_EngineAll(ufast_t ufl_line)
{
    if(tg_pIntLineTable[ufl_line]->int_type == CN_REAL)
        __Int_EngineReal(ufl_line);                //��ʵʱ�ж�
    else
        __Int_EngineAsynSignal(ufl_line);         //���첽�ź�
}
*/

//----ʵʱ�ж�����-------------------------------------------------------------
//���ܣ���Ӧʵʱ�жϣ������жϺŵ����û�ISR
//������ufast ufl_line����Ӧ���ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------
void __Int_EngineReal(ufast_t ufl_line)
{
    struct IntLine *ptIntLine;
    tg_int_global.nest_real++;
    ptIntLine = tg_pIntLineTable[ufl_line];

    //��if�������ֲ���У���cpu���жϹ������ļ��������й�:
    //1���첽�ź��Ƿ��ж����Ŀ��أ�����cortex-m3��omapl138�����еģ�2440��2416��
    //   2410����û�еġ����û�ж������أ����ڴ����ж�ǰ��ϵ��첽�ź��߿���
    //2���첽�źź�ʵʱ�ж϶����ڿ���״̬������£��첽�ź��Ƿ������ռʵʱ�жϡ�
    //   ������ԣ��ǲ���ʵ��ʵʱ�ж�Ƕ�׵ġ�
    //3��ʵʱ�ж���Ӧ���Ƿ��Զ��ر�ʵʱ�ж�
    //4���þ���ʵ���Ƿ�֧��ʵʱ�ж�Ƕ��
    //5����ʵ��֧��ʵʱ�ж�Ƕ��
    if(tg_pIntLineTable[ufl_line]->enable_nest == false)
    {
//        Int_CutTrunk();
    }

    if(tg_pIntLineTable[ufl_line]->ISR != NULL)
        tg_pIntLineTable[ufl_line]->ISR(ptIntLine->para);  //�����û��жϺ���

    if(tg_pIntLineTable[ufl_line]->enable_nest == false)
    {
//        Int_ContactTrunk();
    }
    tg_int_global.nest_real--;
}

//----�첽�¼��ж�����---------------------------------------------------------
//���ܣ���Ӧ�첽�źţ������жϺŵ����û�ISR����󵯳��ж��߿��ƿ��my_evtt_id
//      ��Աָ�����¼����ͣ�����ڷ���ǰ�鿴�Ƿ���Ҫ���������л�������Ҫ���л�
//      ֮��
//������ufast_t ufl_line����Ӧ���ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------
void __Int_EngineAsynSignal(ufast_t ufl_line)
{
    struct EventECB *event;
    struct IntLine *ptIntLine;
    u32 isr_result;

    g_bScheduleEnable = false;
    ptIntLine =tg_pIntLineTable[ufl_line];
    tg_int_global.nest_asyn_signal=1;
    if(ptIntLine->clear_type == CN_INT_CLEAR_AUTO)
        Int_ClearLine(ufl_line);        //�ж�Ӧ��,
    if(ptIntLine->ISR != NULL)
        isr_result = ptIntLine->ISR(ptIntLine->para);
    else
    {
        if(ptIntLine->clear_type == CN_INT_CLEAR_USER)
            Int_ClearLine(ptIntLine->para);        //�ж�Ӧ��,
    }
//    if(ptIntLine->clear_type == CN_INT_CLEAR_POST)
//        Int_ClearLine(ufl_line);        //�ж�Ӧ��,
    event = ptIntLine->sync_event;
    if(event != NULL)   //��ͬ��ָ������û���¼�(ע�������¼������Ƕ���)
    {
        event->event_result = isr_result;
        __DJY_EventReady(event);   //�Ѹ��¼��ŵ�ready����
        ptIntLine->sync_event = NULL;   //���ͬ��
    }
    if(ptIntLine->my_evtt_id != CN_INVALID_EVTT_ID)
    {
        DJY_EventPop(ptIntLine->my_evtt_id,
                        NULL,0,(ptu32_t)isr_result, (ptu32_t)ufl_line,0);
    }
    tg_int_global.nest_asyn_signal = 0;

    if(g_ptEventReady != g_ptEventRunning)
        __DJY_ScheduleAsynSignal();       //ִ���ж��ڵ���
    g_bScheduleEnable = true;
    return;
}

/*
//DAI\DPI�ܵ��ж�����ο����̣����ڵ���DAI\DPI�������жϷ�����

//----DAI�ж�����--------------------------------------------------------------
//���ܣ�DAI�ж�����
//��������
//���أ�ufast_t dai_line(dai�жϺţ���cpu_peri_int_line.h�ж���)
//-----------------------------------------------------------------------------
ufast_t __int_engine_dai( void )
{
    ufast_t dai_line;
    u8 int_cont=0;

    dai_line = (*pDAI_IRPTL_HS);        // read the DAIHI Shadow register
    int_cont = cn_int_leading_zero[(dai_line>>8)&0x000000ff];
    if(int_cont == 8)// all zero
        {
            int_cont = cn_int_leading_zero[(dai_line>>16)&0x000000ff];
            if(int_cont == 8)// all zero
                {
                    // last chance
                    int_cont = cn_int_leading_zero[(dai_line>>24)&0x000000ff];
                    dai_line = 31-int_cont;
                }
            else// stop search
                {
                    dai_line = 23-int_cont;
                }
        }
    else// stop search
        {
            dai_line = 15-int_cont;
        }
    dai_line += cn_int_line_dai_base;           // caculate the real int num

    return dai_line;

}

//----DPI�ж�����--------------------------------------------------------------
//���ܣ�DAI�ж�����
//��������
//���أ�ufast_t dpi_line(dpi�жϺţ���cpu_peri_int_line.h�ж���)
//-----------------------------------------------------------------------------
ufast_t __int_engine_dpi( void )
{
    ufast_t dpi_line;
    u8 int_cont=0;

    dpi_line = (*pDPI_IRPTL_SH);        // read the DPII Shadow register
    int_cont = cn_int_leading_zero[dpi_line&0x000000ff];
    if(int_cont == 8)// all zero
        {
            // last chance
            int_cont = cn_int_leading_zero[(dpi_line>>8)&0x000000ff];
            dpi_line = 15-int_cont;
        }
    else// stop search
        {
            dpi_line = 7-int_cont;
        }
    dpi_line += cn_int_line_dpi_base;       // caculate the real int num

    return dpi_line;

}
*/

