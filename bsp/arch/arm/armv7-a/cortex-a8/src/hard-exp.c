#include "stdio.h"
#include "djyos.h"
#include "int.h"
#include "blackbox.h"
#include "hard-exp.h"
#include "exp_vector.h"
#include "board-config.h"
#include "project_config.h"
void* g_u32ExpTable[8] __attribute__ ((section(".data.ExpTable")));

void Init_Cpu(void);
void AppStart(void);
void __irq_Int_EngineAll(ufast_t intStatus);
void __fiq_Int_EngineAll(ufast_t intStatus);

#if(CFG_RUNMODE == CN_RUNMODE_IBOOT)
void HardExp_UndefVector(ucpu_t *sp)
{
    return;
}
void HardExp_SwiVector(ucpu_t *sp)
{
    return;
}
void HardExp_PrefectVector(ucpu_t *sp)
{
    return;
}
void HardExp_DataAbortVector(ucpu_t *sp)
{
    return;
}
void HardExp_NotUseVector(ucpu_t *sp)
{
    return;
}
//-----------------------------------------------------------------------------
//功能：初始化异常向量表
//参数：无
//返回：无
//-----------------------------------------------------------------------------
void HardExp_Init(void)
{
    g_u32ExpTable[CN_VECTOR_RESET]      = (void*)Init_Cpu;
    g_u32ExpTable[CN_VECTOR_UND]        = (void*)HardExp_UndefVector;
    g_u32ExpTable[CN_VECTOR_SWI]        = (void*)HardExp_SwiVector;
    g_u32ExpTable[CN_VECTOR_PABT]       = (void*)HardExp_PrefectVector;
    g_u32ExpTable[CN_VECTOR_DABT]       = (void*)HardExp_DataAbortVector;
    g_u32ExpTable[CN_VECTOR_NOTUSE]     = (void*)HardExp_NotUseVector;
    g_u32ExpTable[CN_VECTOR_IRQ]        = (void*)__irq_Int_EngineAll;
    g_u32ExpTable[CN_VECTOR_FIQ]        = (void*)__fiq_Int_EngineAll;
}
#else
extern void EnJtag(void);
extern void stub_debug(void);
void HardExp_UndefVector(ucpu_t *sp)
{
    volatile u32 t = 1;
    EnJtag();
    if(t)
        while(1);
    return;
}
void HardExp_SwiVector(ucpu_t *sp)
{
    volatile u32 t = 1;
    EnJtag();
    if(t)
        while(1);
    return;
}
void HardExp_PrefectVector(ucpu_t *sp)
{
    volatile u32 t = 1;
    EnJtag();
    if(t)
        while(1);
    return;
}
void HardExp_DataAbortVector(ucpu_t *sp)
{
    volatile u32 t = 1;
    EnJtag();
    if(t)
        while(1);
    return;
}
void HardExp_NotUseVector(ucpu_t *sp)
{
    volatile u32 t = 1;
    EnJtag();
    if(t)
        while(1);
    return;
}
//-----------------------------------------------------------------------------
//功能：初始化异常向量表
//参数：无
//返回：无
//-----------------------------------------------------------------------------
void HardExp_Init(void)
{
#if(CFG_RUNMODE == CN_RUNMODE_BOOTSELF)
    g_u32ExpTable[CN_VECTOR_RESET]      = (void*)Init_Cpu;
#else
    g_u32ExpTable[CN_VECTOR_RESET]      = (void*)AppStart;
#endif
    g_u32ExpTable[CN_VECTOR_UND]        = (void*)HardExp_UndefVector;
    g_u32ExpTable[CN_VECTOR_SWI]        = (void*)HardExp_SwiVector;
    g_u32ExpTable[CN_VECTOR_PABT]       = (void*)HardExp_PrefectVector;
    g_u32ExpTable[CN_VECTOR_DABT]       = (void*)HardExp_DataAbortVector;
    g_u32ExpTable[CN_VECTOR_NOTUSE]     = (void*)HardExp_NotUseVector;
    g_u32ExpTable[CN_VECTOR_IRQ]        = (void*)__irq_Int_EngineAll;
    g_u32ExpTable[CN_VECTOR_FIQ]        = (void*)__fiq_Int_EngineAll;
}
#endif

bool_t HardExp_InfoDecoderInit(void)
{
    return true;
}