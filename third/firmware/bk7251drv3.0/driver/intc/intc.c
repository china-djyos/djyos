/**
 ****************************************************************************************
 *
 * @file intc.c
 *
 * @brief Definition of the Interrupt Controller (INTCTRL) API.
 *
 * Copyright (C) RivieraWaves 2011-2016
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "compiler.h"
#include "intc.h"
#include "intc_pub.h"

#include "include.h"
#include "arm_arch.h"
#include "drv_model_pub.h"
#include "icu_pub.h"
#include "mem_pub.h"
#include "uart_pub.h"
#include "power_save_pub.h"

ISR_T _isrs[INTC_MAX_COUNT] = {0,};
static UINT32 isrs_mask = 0;
static ISR_LIST_T isr_hdr = {{&isr_hdr.isr, &isr_hdr.isr},};

void intc_hdl_entry(UINT32 int_status)
{
    UINT32 i;
    ISR_T *f;
    UINT32 status;
    LIST_HEADER_T *n;
    LIST_HEADER_T *pos;

    status = int_status & isrs_mask;
    INTC_PRT("intc:%x:%x\r\n", int_status, status);

    #if CFG_USE_STA_PS
    power_save_dtim_wake(status);
    #endif

    list_for_each_safe(pos, n, &isr_hdr.isr)
    {
        f = list_entry(pos, ISR_T, list);
        i = f->int_num;

        if ((BIT(i) & status))
        {
            f->isr_func();
            status &= ~(BIT(i));
        }

        if(0 == status)
        {
            return;
        }
    }
}
#if (CFG_SUPPORT_DJYOS)
#include "cpu_peri_reg.h"
#include "int.h"
#include "uart.h"
#include "pwm.h"
#include "../icu/icu.h"
void intc_service_register(UINT8 int_num, UINT8 int_pri, FUNCPTR isr)
{
    (void)int_pri;
    Int_Register(int_num);
    Int_SetClearType(int_num,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(int_num,isr);
    Int_SettoAsynSignal(int_num);
    Int_ClearLine(int_num);
    Int_RestoreAsynLine(int_num);
}
void Disable_AllPeri()
{
    REG32_CLEAR_BIT(REG_UART1_CONFIG,(UART_TX_ENABLE|UART_RX_ENABLE));
    REG32_CLEAR_BIT(REG_UART2_CONFIG,(UART_TX_ENABLE|UART_RX_ENABLE));
//    REG32_CLEAR_BIT(PWM_CTL,(PWM0_EN_BIT|PWM1_EN_BIT|PWM2_EN_BIT|PWM3_EN_BIT|PWM4_EN_BIT|PWM5_EN_BIT));
    REG32_CLEAR_REG(PWM_CTL);
    REG32_CLEAR_REG(0x0802800+ 34*4);//gpio
    REG32_CLEAR_REG(ICU_INTERRUPT_ENABLE);//disable all icu reg
}
#else      //#if (CFG_SUPPORT_DJYOS)
void intc_service_register(UINT8 int_num, UINT8 int_pri, FUNCPTR isr)
{
    LIST_HEADER_T *pos, *n;
    ISR_T *tmp_ptr, *cur_ptr;
    ISR_T buf_ele;

    GLOBAL_INT_DECLARATION();

    buf_ele           = _isrs[int_num];
    cur_ptr           = &_isrs[int_num];
    cur_ptr->isr_func = isr;
    cur_ptr->int_num  = int_num;
    cur_ptr->pri      = int_pri;

    INTC_PRT("reg_isr:%d:%d:%p\r\n", int_num, int_pri, isr);

    GLOBAL_INT_DISABLE();
    if (list_empty(&isr_hdr.isr))
    {
        list_add_head(&cur_ptr->list, &isr_hdr.isr);
        goto ok;
    }

    /* Insert the ISR to the function list, this list is sorted by priority number */
    list_for_each_safe(pos, n, &isr_hdr.isr)
    {
        tmp_ptr = list_entry(pos, ISR_T, list);

        if (int_pri < tmp_ptr->pri)
        {
            /* add entry at the head of the queue */
            list_add_tail(&cur_ptr->list, &tmp_ptr->list);

            INTC_PRT("reg_isr_o1\r\n");

            goto ok;
        }
        else if (int_pri == tmp_ptr->pri)
        {
            INTC_PRT("reg_isr_error\r\n");
            goto error;
        }
    }

    list_add_tail(&cur_ptr->list, &isr_hdr.isr);
    INTC_PRT("reg_isr_o2\r\n");

ok:
    isrs_mask |= BIT(int_num);
    GLOBAL_INT_RESTORE();

    return;


error:
    /* something wrong  */
    _isrs[int_num] = buf_ele;
    GLOBAL_INT_RESTORE();

    return;
}
#endif     //#if (CFG_SUPPORT_DJYOS)

void intc_service_change_handler(UINT8 int_num, FUNCPTR isr)
{
    LIST_HEADER_T *pos, *n;
    ISR_T *tmp_ptr, *cur_ptr;
    ISR_T buf_ele;
    UINT8 int_pri;

    GLOBAL_INT_DECLARATION();

    buf_ele           = _isrs[int_num];
    cur_ptr           = &_isrs[int_num];
    int_pri = cur_ptr->pri;

    if(!cur_ptr->isr_func)
        return;

    INTC_PRT("reg_isr:%d:%d:%p\r\n", int_num, int_pri, isr);

    GLOBAL_INT_DISABLE();
    if (list_empty(&isr_hdr.isr))
    {
        goto exit;
    }

    /* Insert the ISR to the function list, this list is sorted by priority number */
    list_for_each_safe(pos, n, &isr_hdr.isr)
    {
        tmp_ptr = list_entry(pos, ISR_T, list);

        if (int_pri == tmp_ptr->pri)
        {
            buf_ele.isr_func = isr;
            break;
        }
    }

exit:
    /* something wrong  */
    _isrs[int_num] = buf_ele;
    GLOBAL_INT_RESTORE();

    return;
}


/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
void intc_spurious(void)
{
    ASSERT(0);
}

void intc_enable(int index)
{
    UINT32 param;

    param = (1UL << index);
    sddev_control(ICU_DEV_NAME, CMD_ICU_INT_ENABLE, &param);
}

void intc_disable(int index)
{
    UINT32 param;

    param = (1UL << index);
    sddev_control(ICU_DEV_NAME, CMD_ICU_INT_DISABLE, &param);
}

void rf_ps_wakeup_isr_idle_int_cb()
{
#if ( CONFIG_APP_MP3PLAYER == 1 )
    UINT32 irq_status;

    irq_status = sddev_control(ICU_DEV_NAME, CMD_GET_INTR_STATUS, 0);

    if(irq_status & 1<<IRQ_I2S_PCM)
    {
    irq_status &= 1<<IRQ_I2S_PCM;
    i2s_isr();
    sddev_control(ICU_DEV_NAME, CMD_CLR_INTR_STATUS, &irq_status);
    }
#endif
}

void intc_irq(void)
{
    UINT32 irq_status;

    irq_status = sddev_control(ICU_DEV_NAME, CMD_GET_INTR_STATUS, 0);
    irq_status = irq_status & 0xFFFF;
    if(0 == irq_status)
    {
        #if (! CFG_USE_STA_PS)
        os_printf("irq:dead\r\n");
        #endif
    }

    sddev_control(ICU_DEV_NAME, CMD_CLR_INTR_STATUS, &irq_status);

    intc_hdl_entry(irq_status);
}

void intc_fiq(void)
{
    UINT32 fiq_status;

    fiq_status = sddev_control(ICU_DEV_NAME, CMD_GET_INTR_STATUS, 0);
    fiq_status = fiq_status & 0xFFFF0000;
    sddev_control(ICU_DEV_NAME, CMD_CLR_INTR_STATUS, &fiq_status);

    intc_hdl_entry(fiq_status);
}

#if (CFG_SUPPORT_ALIOS)
void deafult_swi(void)
{
    while(1);
}
#endif

void intc_init(void)
{
    UINT32 param;

#if( ! CFG_SUPPORT_DJYOS )
    *((volatile uint32_t *)0x400000) = (uint32_t)&do_irq;
    *((volatile uint32_t *)0x400004) = (uint32_t)&do_fiq;
    *((volatile uint32_t *)0x400008) = (uint32_t)&do_swi;
    *((volatile uint32_t *)0x40000c) = (uint32_t)&do_undefined;
    *((volatile uint32_t *)0x400010) = (uint32_t)&do_pabort;
    *((volatile uint32_t *)0x400014) = (uint32_t)&do_dabort;
    *((volatile uint32_t *)0x400018) = (uint32_t)&do_reserved;

    intc_enable(FIQ_MAC_GENERAL);
    intc_enable(FIQ_MAC_PROT_TRIGGER);

    intc_enable(FIQ_MAC_TX_TRIGGER);
    intc_enable(FIQ_MAC_RX_TRIGGER);

    intc_enable(FIQ_MAC_TX_RX_MISC);
    intc_enable(FIQ_MAC_TX_RX_TIMER);

    intc_enable(FIQ_MODEM);

#endif         //for #if( ! CFG_SUPPORT_DJYOS )
    param = GINTR_FIQ_BIT | GINTR_IRQ_BIT;
    sddev_control(ICU_DEV_NAME, CMD_ICU_GLOBAL_INT_ENABLE, &param);

    return;
}

void intc_deinit(void)
{
    UINT32 param;

    for( int i = 0; i<=FIQ_DPLL_UNLOCK; i++)
    {
        intc_disable(i);
    }

    param = GINTR_FIQ_BIT | GINTR_IRQ_BIT;
    sddev_control(ICU_DEV_NAME, CMD_ICU_GLOBAL_INT_DISABLE, &param);

    return;
}

//lst 以下函数在 djyos 上用不上
void bk_cpu_shutdown(void)
{
    GLOBAL_INT_DECLARATION();

    os_printf("shutdown...\n");

    GLOBAL_INT_DISABLE();
    while(1);
    GLOBAL_INT_RESTORE();
}

void bk_show_register (struct arm_registers *regs)
{
    os_printf("Execption:\n");
    os_printf("r00:0x%08x r01:0x%08x r02:0x%08x r03:0x%08x\n",
               regs->r0, regs->r1, regs->r2, regs->r3);
    os_printf("r04:0x%08x r05:0x%08x r06:0x%08x r07:0x%08x\n",
               regs->r4, regs->r5, regs->r6, regs->r7);
    os_printf("r08:0x%08x r09:0x%08x r10:0x%08x\n",
               regs->r8, regs->r9, regs->r10);
    os_printf("fp :0x%08x ip :0x%08x\n",
               regs->fp, regs->ip);
    os_printf("sp :0x%08x lr :0x%08x pc :0x%08x\n",
               regs->sp, regs->lr, regs->pc);
    os_printf("cpsr:0x%08x\n", regs->cpsr);
}

void bk_trap_udef(struct arm_registers *regs)
{
    bk_show_register(regs);
    os_printf("undefined instruction\n");
    bk_cpu_shutdown();
}

void bk_trap_pabt(struct arm_registers *regs)
{
    bk_show_register(regs);
    os_printf("prefetch abort\n");
    bk_cpu_shutdown();
}

void bk_trap_dabt(struct arm_registers *regs)
{
    bk_show_register(regs);
    os_printf("data abort\n");
    bk_cpu_shutdown();
}

void bk_trap_resv(struct arm_registers *regs)
{
    os_printf("not used\n");
    bk_show_register(regs);
    bk_cpu_shutdown();
}

/// @}
