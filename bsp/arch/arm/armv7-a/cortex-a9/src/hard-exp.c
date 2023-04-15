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
// 函数功能：此部分主要用来处理处理器的通用异常信息和NVIC的部分带有异常信息的寄存器
// 输入参数：layer,抛出层次
//           parahead，抛出时的参数
//           endian, 信息的存储格式
// 输出参数：无
// 返回值  : true成功, false失败
// =============================================================================
bool_t  HardExp_Decoder(struct BlackBoxThrowPara *parahead, u32 endian)
{
    bool_t result = false;
    struct SysExceptionInfo  *mycpuinfo;

    if(parahead == NULL)
    {
        //非本层信息，无需解析
        printk("CPU核心硬件没有异常\r\n");
        result = false;
    }
    else
    {
        if((sizeof(struct SysExceptionInfo))== parahead->BlackBoxInfoLen)//看看包是否完整
        {   /*
            //基本算是完整包，慢慢解析吧
            mycpuinfo = (struct SysExceptionInfo *)(parahead->para);
            //存储端转换
            if(endian != CN_CFG_BYTE_ORDER)
            {
                __SwapExpCpuInfoByEndian(mycpuinfo);
            }
            if(CN_SYS_EXP_CPUINFO_VALIDFLAG == mycpuinfo->SysExpCpuFlag)//当前版本，可以解析
            {
                printk("CPUINFO LIST:\r\n");
                printk("CPUINFO:len = 0x%08x\n\r",parahead->para_len);
                printk("CPUINFO:deal result = 0x%08x\n\r", parahead->ExpAction);

                //EXP FAULT 信息解析
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
                result = false; //可能不是一样的CPU或者版本，无法解析
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
// 函数: 注册硬件异常的数据解析函数, 所有平台均须提供这个函数,供异常模块调用,否则
//       将导致异常模块编译不通过.
// 参数: 无
// 返回值     :true成功, false失败
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

void *g_u32ExpTable[32] __attribute__ ((section(".data.ExpTable")));
// =============================================================================
// 函数功能：异常初始化,包括异常向量表的初始化
// 输入参数：暂时无（后续待定）
// 输出参数：无
// 返回值  : true成功, false失败
// =============================================================================
void HardExp_Init(void)
{
	int i;
	extern void *g_u32ExpTable[];

	for (i = 0; i < 32; i++) {
		g_u32ExpTable[i] = (void *)0;
	}
	/* PPI Timer */
	g_u32ExpTable[29] = (void *)0;// HardExp_SystickHandler;

#if 0
	/* done in startup.S */
	ldr	r0, =__djy_vectors
	mcr	p15, 0, r0, c12, c0, 0
#endif
}
