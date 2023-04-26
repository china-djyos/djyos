#include "stdint.h"
#include "stdlib.h"
#include "cpu_peri.h"
#include "int.h"
#include "int_hard.h"
#include "critical.h"
#include "djyos.h"

#include <string.h>


/* Distributor register map */
#define ICDDCR 0x000
#define ICDICTR 0x004
#define ICDIIDR 0x008
#define ICDISR 0x080
#define ICDISER 0x100
#define ICDICER 0x180
#define ICDISPR 0x200
#define ICDICPR 0x280
#define ICDABR 0x300
#define ICDIPR 0x400
#define ICDIPTR 0x800
#define ICDICFR 0xC00
#define ICDSGIR 0xF00

/* CPU interface register map */
#define ICCICR 0x00
#define ICCPMR 0x04
#define ICCBPR 0x08
#define ICCIAR 0x0C
#define ICCEOIR 0x10
#define ICCRPR 0x14
#define ICCHPIR 0x18
#define ICCABPR 0x1C
#define ICCIIDR 0xFC

extern struct IntLine *tg_pIntLineTable[CN_INT_LINE_LAST+1]; //�ж��߲��ұ�
extern struct IntMasterCtrl  tg_int_global;          //���岢��ʼ�����жϿ��ƽṹ
extern void __DJY_ScheduleAsynSignal(void);
void __DJY_EventReady(struct EventECB *event_ready);

#define CN_PRIO_ASYN 31
#define CN_PRIO_REAL 30

#define BITS_PER_LONG           32

/* include/linux/bitops.h */
#define BIT(nr)                 (1UL << (nr))
#define BIT_ULL(nr)             (1ULL << (nr))
#define BIT_MASK(nr)            (1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)            ((nr) / BITS_PER_LONG)
#define BIT_ULL_MASK(nr)        (1ULL << ((nr) % BITS_PER_LONG_LONG))
#define BIT_ULL_WORD(nr)        ((nr) / BITS_PER_LONG_LONG)
#define BITS_PER_BYTE           8
#define BITS_TO_LONGS(nr)       DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))

/**
 * __set_bit - Set a bit in memory
 * @nr: the bit to set
 * @addr: the address to start counting from
 *
 * Unlike set_bit(), this function is non-atomic and may be reordered.
 * If it's called on the same region of memory simultaneously, the effect
 * may be that only one operation succeeds.
 */
static inline void generic_set_bit(int nr, volatile unsigned long *addr)
{
    unsigned long mask = BIT_MASK(nr);
    unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

    *p  |= mask;
}

static inline void generic_clear_bit(int nr, volatile unsigned long *addr)
{
    unsigned long mask = BIT_MASK(nr);
    unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

    *p &= ~mask;
}

//----����߼�ԭ�Ӳ���---------------------------------------------------------
//���ܣ�����ǰ���ж�״̬��Ȼ���ֹ���жϡ��߼�ԭ�Ӳ�����ָ�ڼ䲻�����κ�ԭ��
//      ��ϵĲ�����
//      Int_HighAtomStart--int_high_atom_end��������ʹ�ã�
//      ��������������"Int_SaveTrunk--Int_RestoreTrunk"�Ŀ��ٰ汾���ڱ�������
//      ס�Ĵ�����ڣ����������"Int_SaveTrunk--Int_RestoreTrunk",��������
//      ���ÿ��������߳��л��ĺ��������鲻Ҫ�����κ�ϵͳ���á�
//��������
//���أ�ԭ��״̬
//��ע����ֲ����
//-----------------------------------------------------------------------------
atom_high_t Int_HighAtomStart(void)
{
    volatile atom_high_t ret;
    __asm__ volatile(
        "mrs    %0, cpsr \n"
        "cpsid  i \n"
//        "mov    r0, %0 \n"
        : "=r" (ret)
        :
        );
    return ret;
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
    __asm__(
        "msr cpsr_c,%0 \n\t" : :"r"(high)
    );
}

//----����ͼ�ԭ�Ӳ���---------------------------------------------------------
//���ܣ���ȡ��ǰ�첽�źſ���״̬��Ȼ���ֹ�첽�źš��ͼ���ԭ�Ӳ�����������ֹ�ٽ�
//      ���ݵĲ������ʣ������ܻᱻʵʱ�жϴ�ϡ�
//      Int_LowAtomStart---int_low_atom_end��������ʹ�ã���������������
//      "Int_SaveAsynSignal--Int_RestoreAsynSignal"�Ŀ��ٰ汾���ڱ�������ס
//      �Ĵ�����ڣ����������int_restore_asyn_signal��int_save_asyn_signal�е�
//      �κ�һ��������������ÿ��������߳��л��ĺ��������鲻Ҫ�����κ�ϵͳ���á�
//��������
//���أ�ԭ��״̬
//��ע����ֲ����
//-----------------------------------------------------------------------------
atom_low_t Int_LowAtomStart(void)
{
    volatile atom_low_t ret;
//  ret = getPriorityMask();        todo
    setPriorityMask(CN_PRIO_ASYN);
    return ret;

}

//----�뿪�ͼ�ԭ�Ӳ���---------------------------------------------------------
//���ܣ��ָ�����ǰ�첽�ź����״̬��������Ӧ��int_low_atom_start�������ס�
//      Int_LowAtomStart---int_low_atom_end��������ʹ�ã���������������
//      "Int_SaveAsynSignal--Int_RestoreAsynSignal"�Ŀ��ٰ汾���ڱ�������ס
//      �Ĵ�����ڣ����������int_restore_asyn_signal��int_save_asyn_signal�е�
//      �κ�һ��������������ÿ��������߳��л��ĺ��������鲻Ҫ�����κ�ϵͳ���á�
//������low��int_low_atom_start�����ԭ��״̬
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_LowAtomEnd(atom_low_t low)
{
    setPriorityMask(low);
}

//----��ͨ�첽�źſ���---------------------------------------------------------
//���ܣ���ͨ�첽�źſ���,����ܿ��ؽ�ͨ���ж��߿��ؽ�ͨ,���жϽ�������
//      1.���ж�����Ӳ������ʱ,�Ѹÿ��ؽ�ͨ����
//      2.���û�ж���Ӳ������,���ͨ���б�������첽�źŵ��߿���.
//      3.2440���ڵڶ������,�ѷ����������ж�����rINTMSK��Ӧλ��0.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
//English explainning...
// Int: namespace
// Contact: enable
void Int_ContactAsynSignal(void)
{
    setPriorityMask(CN_PRIO_ASYN);
}

//----�Ͽ��첽�źſ���---------------------------------------------------------
//���ܣ��Ͽ��첽�źſ���,���е��첽�źŽ�����ֹ
//      1.���ж�����Ӳ������ʱ,�Ѹÿ��ضϿ�����
//      2.���û�ж���Ӳ������,��Ͽ������첽�źŵ��߿���.
//      3.2440���ڵڶ������,�������첽�ź�����rINTMSK��Ӧλ��1.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
// Int: namespace
// Cut: disconnect or disable
void Int_CutAsynSignal(void)
{
    setPriorityMask(CN_PRIO_REAL);
}

//----��ͨ���жϿ���-----------------------------------------------------------
//���ܣ���ͨ���жϿ���,����cpu������һ���ܿ���,ֱ�Ӳ����ÿ��ؼ���.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
// Contact: enable
// Trunk: ???
// ʵ�֣������������ж�
void Int_ContactTrunk(void)
{
    asm volatile ("cpsie i\n\t");
}

//----�Ͽ����жϿ���---------------------------------------------------------
//���ܣ��Ͽ����жϿ���,����cpu������һ���ܿ���,ֱ�Ӳ����ÿ��ؼ���.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_CutTrunk(void)
{
    asm volatile ("cpsid i\n\t");
}

//----��ͨ�����ж��߿���-------------------------------------------------------
//���ܣ���ͨ�����ж��߿���,���ж��Ƿ�����Ҫ������Ŀ���״̬
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
bool_t Int_ContactLine(ufast_t ufl_line)
{
    enableIntID(ufl_line);
    return true;
}

//----�Ͽ������ж��߿���-------------------------------------------------------
//���ܣ��Ͽ������ж��߿��أ��������жϺ��첽�źſ���״̬��Σ����ж��߱���ֹ
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
/* explain the english naming:
   Int_: namespace
   Cut: disable
   Line: irq source
*/
bool_t Int_CutLine(ufast_t ufl_line)
{
    disableIntID(ufl_line);
    return true;
}

//----Ӧ���жϣ������Ӧ�ж��ߵ��жϹ���״̬-----------------------------------
//���ܣ�Ӳ��Ӧ������Ӧ�Ĺ��ܣ��ṩ����жϹ���Ĳ��������ǰ��������Ӧͬһ�ж���
//      �ĺ����жϣ�����󣬲ſ�����Ӧ�����жϡ�����������ж��߱�����Ϊʵʱ�ж�
//      �����첽�ź��޹�
//������ufast ufl_line��ָ��Ӧ����ж��ߺ�
//���أ���
//��ע����Щ��ϵ�ж���ӦʱӲ��Ӧ�𣬱�����Ϊ�պ�����
//      ����������ֲ���к���
//-----------------------------------------------------------------------------
bool_t Int_ClearLine(ufast_t ufl_line)
{
    writeEOI(ufl_line);
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
    unsigned long *addr;

    /* ICDISPR */
    asm volatile (
        "MRC p15, 4, %0, c15, c0, 0\n\t"
        "orr %0, %0, #0x1200\n\t"
        : "=r" (addr)
    );
    generic_set_bit(ufl_line, addr);
    return true;
}

//----Ӧ��ȫ���жϣ����ȫ���ж��ߵ��жϹ���״̬-------------------------------
//���ܣ�Ӳ��Ӧ������Ӧ�Ĺ��ܣ��ṩ����жϹ���Ĳ���
//������ufast ufl_line��ָ��Ӧ����ж��ߺ�
//���أ���
//��ע����Щ��ϵ�ж���ӦʱӲ��Ӧ�𣬱�����Ϊ�պ�����
//      ����������ֲ���к���
//-----------------------------------------------------------------------------
void __Int_ClearAllLine(void)
{
    volatile unsigned long *p;

    return;


    #define ICDICPR (0x00a00000 + 0x1000 + 0x280);
    p = (void *)ICDICPR;    /* ARM IHI 0048A Page. 4-3 */
    int i;

    for (i=0; i<CN_INT_LINE_LAST; i+=32) {
        p[i] = ~0;
    }
    return true;
}

//----��ѯ�ж�������״̬-------------------------------------------------------
//���ܣ���ѯ�������Ӧ�ж���״̬�������ڲ�ѯʽ�жϳ���
//������ufl_line������ѯ���ж���
//���أ����жϹ��𣬷���true�����򷵻�false
//��ע: ��Ӳ���ṹ���,��Щ�ṹ���ܲ��ṩ�������,����!
//      ����������ֲ�ؼ�����
//-----------------------------------------------------------------------------
bool_t Int_QueryLine(ufast_t ufl_line)
{
    unsigned long *addr;

    /* ICDISPR */
    asm volatile (
        "MRC p15, 4, %0, c15, c0, 0\n\t"
        "orr %0, %0, #0x1200\n\t"
        : "=r" (addr)
    );
    return addr[ufl_line/32] & (1<<(ufl_line%32));
}

//----��ָ���ж�������Ϊ�첽�ź�--------������---------------------------------
//���ܣ���ָ���ж�������Ϊ�첽�ź�,���ж�������Ӧ,��ǰ�жϷ��غ���Ч
//������ufast ufl_line��ָ�������õ��ж��ߺ�
//���أ�true=�ɹ���false=ʧ��
//      ��������ֲ����
//-----------------------------------------------------------------------------
bool_t Int_SettoAsynSignal(ufast_t ufl_line)
{
    tg_pIntLineTable[ufl_line]->int_type = CN_ASYN_SIGNAL;
    generic_clear_bit(ufl_line, tg_int_global.property_bitmap);

    setIntPriority(ufl_line, CN_PRIO_ASYN);
    return true;
}

//----��ָ���ж�������Ϊʵʱ�ж�--------������---------------------------------
//���ܣ���ָ���ж�������Ϊʵʱ�ж�,���ж�������Ӧ,��ǰ�жϷ��غ���Ч
//������ufast ufl_line��ָ�������õ��ж��ߺ�
//���أ�true=�ɹ���false=ʧ��
//      ��������ֲ����
//-----------------------------------------------------------------------------
bool_t Int_SettoReal(ufast_t ufl_line)
{
    tg_pIntLineTable[ufl_line]->int_type = CN_REAL;
    tg_pIntLineTable[ufl_line]->enable_nest = false;
    generic_set_bit(ufl_line, tg_int_global.property_bitmap);

    setIntPriority(ufl_line, CN_PRIO_REAL);
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
//����: ufl_line�����������ж���
//����: ��
//-----------------------------------------------------------------------------
bool_t Int_EnableNest(ufast_t ufl_line)
{
    return false;
}

//----��ֹ�ж�Ƕ��-------------------------------------------------------------
//����: ���ĳ�жϺŵ��ж�Ƕ��ʹ�ܡ�
//����: ufl_line�����������ж���
//����: ��
//-----------------------------------------------------------------------------
bool_t Int_DisableNest(ufast_t ufl_line)
{
    return true;
}

//----�趨��ռ���ȼ�-----------------------------------------------------------
//����: �趨ָ���ж��ߵ�Ƕ�����ȼ�����������������Ӳ�����ܡ����Ӳ����֧�֣��ɷ���false
//������ufast ufl_line��ָ�������õ��ж��ߺ�
//���أ���
//ע: ��������ֲ����
//-----------------------------------------------------------------------------
bool_t Int_SetPrio(ufast_t ufl_line,u32 prio)
{
    setIntPriority(ufl_line, prio);
    return true;
}

//----��ʼ���ж�Ӳ����ز���---------------------------------------------------
//����: �����
//����: ��
//����: ��
//ע: ��ֲ���У���Ӳ����أ�Ҳ����������й�
//-----------------------------------------------------------------------------
// initial /irqchip
void __Int_InitHard(void)
{
    int_disable_arm_int();     //��ֹ�ж�
    enableGIC();
    enableGICProcessorInterface();
//    pg_int_reg->INTMSK1 = CN_INT_MASK_ALL_LINE;
//�жϹ�������Fλ���ǽ�ֹ,2440���жϹ�������ȱ�ݣ�����ֻ����һ
//���ж�����Ϊfiq������fiq����ж��ߵĹ���ʹ��fiqʵ�ʱ�irq�����ʲ�ʹ��fiq��
//�����첽�źŻ���ʵʩ�жϣ���ʹ��irqʵ�֡�
// ���ж���������
//  pg_int_reg->INTMOD1=0;    //���������ж���ΪIRQ�ж�
//  engine_irq = __Int_EngineAll;
//  irq_bottom = __Int_EngineAll;
}
//----��ʼ���ж�---------------------------------------------------------------
//���ܣ���ʼ���ж�Ӳ��,��ʼ���ж������ݽṹ
//      2.�첽�źű��ֽ�ֹ,��������������������д�.
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
void Int_Init(void)
{
    int i;
    memset(tg_pIntLineTable, 0, sizeof(tg_pIntLineTable));
    memset(&tg_int_global, 0, sizeof(struct IntMasterCtrl));
    tg_int_global.en_asyn_signal_counter = 1;

    enableGIC();
    enableGICProcessorInterface();
    setBinaryPoint(2);
    Int_CutAsynSignal();
    Int_ContactTrunk();
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

void __Int_EngineAll(ufast_t ufl_line)
{
    if(tg_pIntLineTable[ufl_line]->int_type == CN_REAL)
        __Int_EngineReal(ufl_line);                //��ʵʱ�ж�
    else
        __Int_EngineAsynSignal(ufl_line);         //���첽�ź�
}

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
    //   �������ʵ�����ǲ���ʵ��ʵʱ�ж�Ƕ�׵ġ�
    //3��ʵʱ�ж���Ӧ���Ƿ��Զ��ر�ʵʱ�ж�
    //4���þ���ʵ���Ƿ�֧��ʵʱ�ж�Ƕ��
    //5����ʵ�ֲ�֧��ʵʱ�ж�Ƕ�ף�������ע�͵�
//    if(ptIntLine->enable_nest == true)
//    {
//        Int_DisableRealLine(ufl_line);    //����ֹ�����ܻ��γ�����ѭ��
//        Int_ContactTrunk();
//    }
    if(ptIntLine->ISR != NULL)
        ptIntLine->ISR(ptIntLine->para);  //�����û��жϺ���

//    if(ptIntLine->enable_nest == true)  //֧��Ƕ�׵����
//    {
//        Int_CutTrunk();
//        Int_EnableRealLine(ufl_line);
//    }

    //�����ʵ��֧��ʵʱ�ж�Ƕ�ף���������4��
    tg_int_global.nest_real--;

}

//----�첽�¼��ж�����---------------------------------------------------------
//���ܣ���Ӧ�첽�źţ������жϺŵ����û�ISR����󵯳��ж��߿��ƿ��my_evtt_id
//      ��Աָ�����¼����ͣ�����ڷ���ǰ�鿴�Ƿ���Ҫ���������л�������Ҫ���л�
//      ֮��
//������ufast ufl_line����Ӧ���ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------
void __Int_EngineAsynSignal(ufast_t ufl_line)
{
    struct EventECB *event;
    struct IntLine *ptIntLine;
    u32 isr_result;

    tg_int_global.en_asyn_signal_counter = 1;    //�޶����첽�źſ��صģ������
    g_bScheduleEnable = false;
    ptIntLine = tg_pIntLineTable[ufl_line];
//    tg_int_global.en_asyn_signal = false;
    tg_int_global.nest_asyn_signal++;
    if(ptIntLine->enable_nest == true)  //����Ƕ�׵����
    {
        Int_DisableAsynLine(ufl_line);  //����ֹ�����ܻ��γ�����ѭ��
        Int_ContactAsynSignal();
    }
    else
        Int_CutAsynSignal();
    if(ptIntLine->clear_type == CN_INT_CLEAR_AUTO)
        Int_ClearLine(ufl_line);        //�ж�Ӧ��,
    Int_ContactTrunk();
    if(ptIntLine->ISR != NULL)
        isr_result = ptIntLine->ISR(ptIntLine->para);
    else
    {
        if(ptIntLine->clear_type == CN_INT_CLEAR_USER)
            Int_ClearLine(ufl_line);        //�ж�Ӧ��,
    }
//    if(ptIntLine->clear_type == CN_INT_CLEAR_POST)
//        Int_ClearLine(ufl_line);        //�ж�Ӧ��,
    Int_CutAsynSignal();
    Int_EnableAsynLine(ufl_line);

    event = ptIntLine->sync_event;
    if(event != NULL)   //��ͬ��ָ������û���¼�(ע�������¼������Ƕ���)
    {
        event->event_result = isr_result;
        __DJY_EventReady(event);   //�Ѹ��¼��ŵ�ready����
        ptIntLine->sync_event = NULL;   //���ͬ��
    }
    if(ptIntLine->my_evtt_id != CN_EVTT_ID_INVALID)
    {
        DJY_EventPop(ptIntLine->my_evtt_id,
                        NULL,0,(ptu32_t)isr_result, (ptu32_t)ufl_line,0);
    }
    tg_int_global.nest_asyn_signal--;
    if(tg_int_global.nest_asyn_signal == 0)
    {
//        tg_int_global.en_asyn_signal = true;
        tg_int_global.en_asyn_signal_counter = 0; //�޶����첽�źſ��صģ������
        if(g_ptEventReady != g_ptEventRunning)
            __DJY_ScheduleAsynSignal();       //ִ���ж��ڵ���
        g_bScheduleEnable = true;
    }
    Int_CutTrunk();
    Int_ContactAsynSignal();
}


