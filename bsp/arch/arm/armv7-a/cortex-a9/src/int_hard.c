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

extern struct IntLine *tg_pIntLineTable[CN_INT_LINE_LAST+1]; //中断线查找表
extern struct IntMasterCtrl  tg_int_global;          //定义并初始化总中断控制结构
extern void __DJY_ScheduleAsynSignal(void);
void __DJY_EventReady(struct EventECB *event_ready);

#define CN_PRIO_ASYN 30
#define CN_PRIO_REAL 29
#define CN_PRIO_DIS_ASYN 30
#define CN_PRIO_ENA_ASYN 31


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

//----进入高级原子操作---------------------------------------------------------
//功能：读当前总中断状态，然后禁止总中断。高级原子操作是指期间不容许任何原因
//      打断的操作。
//      Int_HighAtomStart--int_high_atom_end必须配套使用，
//      这两函数可视作"Int_SaveTrunk--Int_RestoreTrunk"的快速版本，在被他们套
//      住的代码块内，不允许调用"Int_SaveTrunk--Int_RestoreTrunk",更不允许
//      调用可能引起线程切换的函数，建议不要调用任何系统调用。
//参数：无
//返回：原子状态
//备注：移植敏感
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

//----离开高级原子操作---------------------------------------------------------
//功能：恢复进入离开原子操作前的总中断状态，本函数应与int_high_atom_start函数配套
//      Int_HighAtomStart--int_high_atom_end必须配套使用，在被他们套住的代码块内不
//     允许调用用任何系统调用。
//参数：high，int_high_atom_start保存的原子状态
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
void Int_HighAtomEnd(atom_high_t high)
{
    __asm__(
        "msr cpsr_c,%0 \n\t" : :"r"(high)
    );
}

//----进入低级原子操作---------------------------------------------------------
//功能：读取当前异步信号开关状态，然后禁止异步信号。低级级原子操作可用来防止临界
//      数据的并发访问，但可能会被实时中断打断。
//      Int_LowAtomStart---int_low_atom_end必须配套使用，这两函数可视作
//      "Int_SaveAsynSignal--Int_RestoreAsynSignal"的快速版本，在被他们套住
//      的代码块内，不允许调用int_restore_asyn_signal、int_save_asyn_signal中的
//      任何一个。更不允许调用可能引起线程切换的函数，建议不要调用任何系统调用。
//参数：无
//返回：原子状态
//备注：移植敏感
//-----------------------------------------------------------------------------
atom_low_t Int_LowAtomStart(void)
{
    volatile atom_low_t ret;
    ret = getPriorityMask();
    setPriorityMask(CN_PRIO_DIS_ASYN);
    return ret;

}

//----离开低级原子操作---------------------------------------------------------
//功能：恢复进入前异步信号许可状态，本函数应与int_low_atom_start函数配套。
//      Int_LowAtomStart---int_low_atom_end必须配套使用，这两函数可视作
//      "Int_SaveAsynSignal--Int_RestoreAsynSignal"的快速版本，在被他们套住
//      的代码块内，不允许调用int_restore_asyn_signal、int_save_asyn_signal中的
//      任何一个。更不允许调用可能引起线程切换的函数，建议不要调用任何系统调用。
//参数：low，int_low_atom_start保存的原子状态
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
void Int_LowAtomEnd(atom_low_t low)
{
    setPriorityMask(low);
}

//----接通异步信号开关---------------------------------------------------------
//功能：接通异步信号开关,如果总开关接通且中断线开关接通,该中断将被允许
//      1.当有独立的硬件开关时,把该开关接通即可
//      2.如果没有独立硬件开关,则接通所有被允许的异步信号的线开关.
//      3.2440属于第二种情况,把符合条件的中断线在rINTMSK相应位清0.
//参数：无
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
//English explainning...
// Int: namespace
// Contact: enable
void Int_ContactAsynSignal(void)
{
    setPriorityMask(CN_PRIO_ENA_ASYN);
}

//----断开异步信号开关---------------------------------------------------------
//功能：断开异步信号开关,所有的异步信号将被禁止
//      1.当有独立的硬件开关时,把该开关断开即可
//      2.如果没有独立硬件开关,则断开所有异步信号的线开关.
//      3.2440属于第二种情况,把所有异步信号线在rINTMSK相应位置1.
//参数：无
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
// Int: namespace
// Cut: disconnect or disable
void Int_CutAsynSignal(void)
{
    setPriorityMask(CN_PRIO_DIS_ASYN);
}

//----接通总中断开关-----------------------------------------------------------
//功能：接通总中断开关,所有cpu都会有一个总开关,直接操作该开关即可.
//参数：无
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
// Contact: enable
// Trunk: ???
// 实现，处理器启用中断
void Int_ContactTrunk(void)
{
    asm volatile ("cpsie i\n\t");
}

//----断开总中断开关---------------------------------------------------------
//功能：断开总中断开关,所有cpu都会有一个总开关,直接操作该开关即可.
//参数：无
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
void Int_CutTrunk(void)
{
    asm volatile ("cpsid i\n\t");
}

//----接通单个中断线开关-------------------------------------------------------
//功能：接通单个中断线开关,该中断是否允许还要看后面的开关状态
//参数：无
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
bool_t Int_ContactLine(ufast_t ufl_line)
{
    enableIntID(ufl_line);
    return true;
}

//----断开单个中断线开关-------------------------------------------------------
//功能：断开单个中断线开关，无论总中断和异步信号开关状态如何，该中断线被禁止
//参数：无
//返回：无
//备注：移植敏感
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

//----应答中断，清除相应中断线的中断挂起状态-----------------------------------
//功能：硬件应该有相应的功能，提供清除中断挂起的操作，清除前，不能响应同一中断线
//      的后续中断，清除后，才可以响应后续中断。本函数与该中断线被设置为实时中断
//      还是异步信号无关
//参数：ufast ufl_line，指定应答的中断线号
//返回：无
//备注：有些体系中断响应时硬件应答，本函数为空函数。
//      本函数是移植敏感函数
//-----------------------------------------------------------------------------
bool_t Int_ClearLine(ufast_t ufl_line)
{
    writeEOI(ufl_line);
    return true;
}

//----激发中断-----------------------------------------------------------------
//功能: 触发一个中断.如果中断本已悬挂,本函数无影响.本函数与该中断线被设置为实时
//      中断还是异步信号无关
//参数：ufast ufl_line，欲触发的中断线号
//返回：如果相应的中断线硬件不提供用软件触发中断功能,返回 false,否则返回 true
//备注: 本函数实现依赖于硬件,有些硬件系统不支持此功能.
//      本函数是移植敏感函数
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

//----应答全部中断，清除全部中断线的中断挂起状态-------------------------------
//功能：硬件应该有相应的功能，提供清除中断挂起的操作
//参数：ufast ufl_line，指定应答的中断线号
//返回：无
//备注：有些体系中断响应时硬件应答，本函数为空函数。
//      本函数是移植敏感函数
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

//----查询中断线请求状态-------------------------------------------------------
//功能：查询并清除相应中断线状态，可用于查询式中断程序
//参数：ufl_line，欲查询的中断线
//返回：若中断挂起，返回true，否则返回false
//备注: 与硬件结构相关,有些结构可能不提供这个功能,慎用!
//      本函数是移植关键函数
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

//----把指定中断线设置为异步信号--------－－－---------------------------------
//功能：把指定中断线设置为异步信号,若中断正在响应,则当前中断返回后生效
//参数：ufast ufl_line，指定被设置的中断线号
//返回：true=成功，false=失败
//      本函数移植敏感
//-----------------------------------------------------------------------------
bool_t Int_SettoAsynSignal(ufast_t ufl_line)
{
    tg_pIntLineTable[ufl_line]->int_type = CN_ASYN_SIGNAL;
    generic_clear_bit(ufl_line, tg_int_global.property_bitmap);

    setIntPriority(ufl_line, CN_PRIO_ASYN);
    return true;
}

//----把指定中断线设置为实时中断--------－－－---------------------------------
//功能：把指定中断线设置为实时中断,若中断正在响应,则当前中断返回后生效
//参数：ufast ufl_line，指定被设置的中断线号
//返回：true=成功，false=失败
//      本函数移植敏感
//-----------------------------------------------------------------------------
bool_t Int_SettoReal(ufast_t ufl_line)
{
    tg_pIntLineTable[ufl_line]->int_type = CN_REAL;
    tg_pIntLineTable[ufl_line]->enable_nest = false;
    generic_set_bit(ufl_line, tg_int_global.property_bitmap);

    setIntPriority(ufl_line, CN_PRIO_REAL);
    return true;
}

//----使能中断嵌套-------------------------------------------------------------
//功能: 使能一个中断源被抢占，使能后，相应的中断服务期间，可能会被别的中断抢占。
//      本函数不分实时中断还是异步信号，与具体的实现有关，移植者应该根据硬件特性
//      和应用需求，确定是否允许嵌套。
//      嵌套发生在实时中断之间或者异步信号之间。实时中断永远可以打断异步信号，这
//      不算嵌套。
//      对于三星系列的ARM芯片中断管理器的特点，对于实时中断的处理，有三种方式:
//      1、用fiq实现实时中断，INTOFFSET1和INTOFFSET2寄存器提供了当前正在服务的
//         中断号，但该两个寄存器在fiq中无效，如果实时中断用fiq实现，则需要逐位
//         检查被设为实时中断的中断号在INTPND1和INTPND1寄存器中的值，才能确定被
//         服务的中断号，这个过程可能非常漫长。这种情况能够实现实时中断嵌套，但
//         却完全失去了实时中断的设计初衷，djyos for s3c2416不采取这种方式。
//      2、有一个特殊情况是，系统只需要一个实时中断，其他全部设为异步信号，这种
//         情况，用fiq实现实时中断是最为理想的，当然只有一个中断，嵌套也就无从
//         谈起了。
//      3、实时中断和异步信号都是用irq方式实现，cpsr的I位用做trunk开关，异步信号
//         没有独立开关，是通过把所有的属于异步信号的中断号的开关全部关掉来模拟
//         异步信号开关的。这种情况是不能实现实时中断嵌套的。为什么呢?arm被设计
//         为，只要I位为0，新的中断就可以抢占正在服务的中断，也就意味着，异步信
//         号抢占实时中断，在硬件上是允许的。实时中断要实现嵌套，须先关掉异步信
//         号，再设置I=0，如果实时中断响应后，在异步信号被关掉之前，紧接着有异步
//         信号发生的话，该irq请求就会发出，随后关掉异步信号也没用，总中断一旦被
//         允许，该异步信号就会抢占实时中断。这种抢占，不仅把"辈分"搞乱，还会引
//         发严重问题，甚至使系统崩溃。如果抢占实时中断的异步信号，服务期间发生
//         了线程切换，把fiq服务的栈，当作pg_event_running的栈。因此，djyos for
//         2416版本不支持实时中断嵌套。
//参数: ufl_line，被操作的中断线
//返回: true = 设置成功，false = 参数错误，或具体实现不支持该中断被嵌套
//-----------------------------------------------------------------------------
bool_t Int_EnableNest(ufast_t ufl_line)
{
    return false;
}

//----禁止中断嵌套-------------------------------------------------------------
//功能: 清除某中断号的中断嵌套使能。
//参数: ufl_line，被操作的中断线
//返回: true = 设置成功，false = 参数错误，或具体实现不支持该中断被嵌套
//-----------------------------------------------------------------------------
bool_t Int_DisableNest(ufast_t ufl_line)
{
    return false;
}

//----设定抢占优先级-----------------------------------------------------------
//功能: 设定指定中断线的嵌套优先级，本函数严重依赖硬件功能。如果硬件不支持，可返回false
//参数：ufast ufl_line，指定被设置的中断线号
//返回：无
//注: 本函数移植敏感
//-----------------------------------------------------------------------------
bool_t Int_SetPrio(ufast_t ufl_line,u32 prio)
{
    setIntPriority(ufl_line, prio);
    return true;
}

//----初始化中断硬件相关部分---------------------------------------------------
//功能: 如标题
//参数: 无
//返回: 无
//注: 移植敏感，与硬件相关，也与软件策略有关
//-----------------------------------------------------------------------------
// initial /irqchip
void __Int_InitHard(void)
{
    int_disable_arm_int();     //禁止中断
    enableGIC();
    enableGICProcessorInterface();
//    pg_int_reg->INTMSK1 = CN_INT_MASK_ALL_LINE;
//中断管理器的F位总是禁止,2440的中断管理器有缺陷，除非只允许一
//个中断配置为fiq，否则fiq检出中断线的过程使得fiq实际比irq慢，故不使用fiq，
//不管异步信号还是实施中断，均使用irq实现。
// 在中断中引用了
//  pg_int_reg->INTMOD1=0;    //设置所有中断线为IRQ中断
//  engine_irq = __Int_EngineAll;
//  irq_bottom = __Int_EngineAll;
}
//----初始化中断---------------------------------------------------------------
//功能：初始化中断硬件,初始化中断线数据结构
//      2.异步信号保持禁止,它会在虚拟机启动引擎中打开.
//      3.总中断允许，
//      用户初始化过程应该遵守如下规则:
//      1.系统开始时就已经禁止所有异步信号,用户初始化时无须担心异步信号发生.
//      2.初始化过程中如果需要操作总中断/实时中断/异步信号,应该成对使用.禁止使
//        异步信号实际处于允许状态(即异步和总中断开关同时允许).
//      3.可以操作中断线,比如连接、允许、禁止等,但应该成对使用.
//      4.建议使用save/restore函数对,不要使用enable/disable函数对.
//参数：无
//返回：无
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

//----总中断引擎---------------------------------------------------------------
//功能：有些系统，在中断向量表部分难于区分实时中断还是异步信号的，或者不希望在汇
//      编阶段使用过于复杂的代码的，比如2416、2440等，则在汇编阶段获取中断号后，
//      直接调用本函数，由本函数再区别调用异步信号引擎或实时中断引擎。
//      像cortex-m3、omapl138这样，在中断向量表部分就可以区别实时中断还是异步信
//      号，则无须提供本函数，而是在汇编部分直接调用相应的引擎。
//参数：ufast ufl_line，响应的中断线号
//返回：无
//-----------------------------------------------------------------------------

void __Int_EngineAll(ufast_t ufl_line)
{
    if(tg_pIntLineTable[ufl_line]->int_type == CN_REAL)
        __Int_EngineReal(ufl_line);                //是实时中断
    else
        __Int_EngineAsynSignal(ufl_line);         //是异步信号
}

//----实时中断引擎-------------------------------------------------------------
//功能：响应实时中断，根据中断号调用用户ISR
//参数：ufast ufl_line，响应的中断线号
//返回：无
//-----------------------------------------------------------------------------
void __Int_EngineReal(ufast_t ufl_line)
{
    struct IntLine *ptIntLine;
    tg_int_global.nest_real++;
    ptIntLine = tg_pIntLineTable[ufl_line];

    //本if语句与移植敏感，跟cpu的中断管理器的几个特性有关:
    //1、异步信号是否有独立的开关，例如cortex-m3、omapl138等是有的，2440、2416、
    //   2410等是没有的。如果没有独立开关，则在打开总中断前须断掉异步信号线开关
    //2、异步信号和实时中断都处于开启状态的情况下，异步信号是否可能抢占实时中断。
    //   这种情况实际上是不能实现实时中断嵌套的。
    //3、实时中断响应后，是否自动关闭实时中断
    //4、该具体实现是否支持实时中断嵌套
    //5、本实现不支持实时中断嵌套，故整个注释掉
//    if(ptIntLine->enable_nest == true)
//    {
//        Int_DisableRealLine(ufl_line);    //不禁止，可能会形成无限循环
//        Int_ContactTrunk();
//    }
    if(ptIntLine->ISR != NULL)
        ptIntLine->ISR(ptIntLine->para);  //调用用户中断函数

//    if(ptIntLine->enable_nest == true)  //支持嵌套的情况
//    {
//        Int_CutTrunk();
//        Int_EnableRealLine(ufl_line);
//    }

    //如果该实现支持实时中断嵌套，启用以下4句
    tg_int_global.nest_real--;

}
void ____Int_EngineAll(ufast_t ufl_line)
{
//    Int_CutAsynSignal();            //1
//    Int_ClearLine(ufl_line);        //2
//    Int_ContactTrunk();             //2
//    Int_CutAsynSignal();            //3
//    Int_EnableLine(ufl_line);       //3
//    Int_CutTrunk();                 //1
//    Int_ContactAsynSignal();        //1
    return;
}

//----异步事件中断引擎---------------------------------------------------------
//功能：响应异步信号，根据中断号调用用户ISR，随后弹出中断线控制块的my_evtt_id
//      成员指定的事件类型，最后在返回前查看是否需要做上下文切换，如需要则切换
//      之。
//参数：ufast ufl_line，响应的中断线号
//返回：无
//-----------------------------------------------------------------------------
void __Int_EngineAsynSignal(ufast_t ufl_line)
{
    struct EventECB *event;
    struct IntLine *ptIntLine;
    u32 isr_result;

    tg_int_global.en_asyn_signal_counter = 1;    //无独立异步信号开关的，需这句
    g_bScheduleEnable = false;
    ptIntLine = tg_pIntLineTable[ufl_line];
//    tg_int_global.en_asyn_signal = false;
    tg_int_global.nest_asyn_signal++;
    if(ptIntLine->enable_nest == true)  //允许嵌套的情况
    {
        Int_DisableAsynLine(ufl_line);  //不禁止，可能会形成无限循环
        Int_ContactAsynSignal();
    }
    else
        Int_CutAsynSignal();
    if(ptIntLine->clear_type == CN_INT_CLEAR_AUTO)
        Int_ClearLine(ufl_line);        //中断应答,
    Int_ContactTrunk();
    if(ptIntLine->ISR != NULL)
        isr_result = ptIntLine->ISR(ptIntLine->para);
    else
    {
        if(ptIntLine->clear_type == CN_INT_CLEAR_USER)
            Int_ClearLine(ufl_line);        //中断应答,
    }
    if(ptIntLine->enable_nest == true)  //允许嵌套的情况
    {
        Int_CutAsynSignal();
        Int_EnableLine(ufl_line);
    }

    event = ptIntLine->sync_event;
    if(event != NULL)   //看同步指针中有没有事件(注：单个事件，不是队列)
    {
        event->event_result = isr_result;
        __DJY_EventReady(event);   //把该事件放到ready队列
        ptIntLine->sync_event = NULL;   //解除同步
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
        tg_int_global.en_asyn_signal_counter = 0; //无独立异步信号开关的，需这句
        if(g_ptEventReady != g_ptEventRunning)
            __DJY_ScheduleAsynSignal();       //执行中断内调度
        g_bScheduleEnable = true;
    }
    Int_CutTrunk();
    Int_ContactAsynSignal();
}


