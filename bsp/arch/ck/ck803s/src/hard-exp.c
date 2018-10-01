//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 钁椾綔鏉冪敱娣卞湷楣忕憺杞欢鏈夐檺鍏徃鎵�鏈夈�傝憲浣滄潈浜轰繚鐣欎竴鍒囨潈鍒┿��
//
// 杩欎唤鎺堟潈鏉℃锛屽湪浣跨敤鑰呯鍚堜笅鍒楁潯浠剁殑鎯呭舰涓嬶紝鎺堜簣浣跨敤鑰呬娇鐢ㄥ強鍐嶆暎鎾湰
// 杞欢鍖呰鍘熷鐮佸強浜岃繘浣嶅彲鎵ц褰㈠紡鐨勬潈鍒╋紝鏃犺姝ゅ寘瑁呮槸鍚︾粡鏀逛綔鐨嗙劧锛�
//
// 1. 瀵逛簬鏈蒋浠舵簮浠ｇ爜鐨勫啀鏁ｆ挱锛屽繀椤讳繚鐣欎笂杩扮殑鐗堟潈瀹ｅ憡銆佹湰鏉′欢鍒楄〃锛屼互
//    鍙婁笅杩扮殑鍏嶈矗澹版槑銆�
// 2. 瀵逛簬鏈浠朵簩杩涗綅鍙墽琛屽舰寮忕殑鍐嶆暎鎾紝蹇呴』杩炲甫浠ユ枃浠朵互鍙婏紡鎴栬�呭叾浠栭檮
//    浜庢暎鎾寘瑁呬腑鐨勫獟浠嬫柟寮忥紝閲嶅埗涓婅堪涔嬬増鏉冨鍛娿�佹湰鏉′欢鍒楄〃锛屼互鍙婁笅杩�
//    鐨勫厤璐ｅ０鏄庛��

// 鍏嶈矗澹版槑锛氭湰杞欢鏄湰杞欢鐗堟潈鎸佹湁浜轰互鍙婅础鐚�呬互鐜扮姸锛�"as is"锛夋彁渚涳紝
// 鏈蒋浠跺寘瑁呬笉璐熶换浣曟槑绀烘垨榛樼ず涔嬫媴淇濊矗浠伙紝鍖呮嫭浣嗕笉闄愪簬灏遍�傚敭鎬т互鍙婄壒瀹氱洰
// 鐨勭殑閫傜敤鎬т负榛樼ず鎬ф媴淇濄�傜増鏉冩寔鏈変汉鍙婃湰杞欢涔嬭础鐚�咃紝鏃犺浠讳綍鏉′欢銆�
// 鏃犺鎴愬洜鎴栦换浣曡矗浠讳富涔夈�佹棤璁烘璐ｄ换涓哄洜鍚堢害鍏崇郴銆佹棤杩囧け璐ｄ换涓讳箟鎴栧洜闈炶繚
// 绾︿箣渚垫潈锛堝寘鎷繃澶辨垨鍏朵粬鍘熷洜绛夛級鑰岃捣锛屽浜庝换浣曞洜浣跨敤鏈蒋浠跺寘瑁呮墍浜х敓鐨�
// 浠讳綍鐩存帴鎬с�侀棿鎺ユ�с�佸伓鍙戞�с�佺壒娈婃�с�佹儵缃氭�ф垨浠讳綍缁撴灉鐨勬崯瀹筹紙鍖呮嫭浣嗕笉闄�
// 浜庢浛浠ｅ晢鍝佹垨鍔冲姟涔嬭喘鐢ㄣ�佷娇鐢ㄦ崯澶便�佽祫鏂欐崯澶便�佸埄鐩婃崯澶便�佷笟鍔′腑鏂瓑绛夛級锛�
// 涓嶈礋浠讳綍璐ｄ换锛屽嵆鍦ㄨ绉嶄娇鐢ㄥ凡鑾蜂簨鍓嶅憡鐭ュ彲鑳戒細閫犳垚姝ょ被鎹熷鐨勬儏褰笅浜︾劧銆�
//-----------------------------------------------------------------------------
#include "stdio.h"
#include "djyos.h"
#include "int.h"
#include "blackbox.h"
#include "csi_core.h"
#include "silan_irq.h"

#include "board-config.h"

#undef  VIC_TSPR
#define VIC_TSPR  0xE000EC10

extern struct IntMasterCtrl  tg_int_global;          //閻庤鐭粻鐔肩嵁鐠哄搫鐏ュ┑顔碱儏鐎垫煡骞�鐠佸疇鍘柡鍌ゅ幗鐢爼宕氶崜浣烘尝闁哄鎷�
extern void __Djy_ScheduleAsynSignal(void);
extern   void Init_Cpu(void);
extern   void HardExp_HardfaultHandler(void);
extern   void CK_Trap0ISR(void);
#if (CN_USE_TICKLESS_MODE)
extern   u32 __Djy_GetTimeBaseReload(void);
#endif
extern u32 __Vectors_Tbl[];

void (*user_systick)(u32 inc_ticks);

#define readl(addr) \
    ({ unsigned int __v = (*(volatile unsigned int *) (addr)); __v; })

static void __Exp_TableSet(void)
{
    __set_VBR((uint32_t) & (__Vectors_Tbl));
    /* Here we may setting exception vector, MGU, cache, and so on. */
    csi_cache_set_range(0, 0x00000000, CACHE_CRCR_4M, 1);
    csi_cache_set_range(1, 0x22000000, CACHE_CRCR_8M, 1);
    csi_dcache_enable();
    csi_icache_enable();

    //*((uint32_t*)VIC_TSPR) = 0xb00;/*濞撴艾顑囬埢鍏肩▔婵犲嫭鐣遍柛瀣绾爼鏁嶇仦鎯ь杹闁告劕濂旂粭鍌氼灉閵堝懐妲戦悗娑櫭▍鎺楀礃濞嗘垶绲诲ǎ鍥ㄧ箘閺嗭拷/

}


void HardExp_ConnectSystick(void (*tick)(u32 inc_ticks))
{
    user_systick = tick;
}

void Exp_SystickTickHandler(void)
{
#if (CN_USE_TICKLESS_MODE)
    u32 tick=0;
#endif
    g_bScheduleEnable = false;
    csi_vic_clear_pending_irq(PIC_IRQID_CTIMER);
    readl(0xE000E010);
//    tg_int_global.en_asyn_signal = false;
    tg_int_global.en_asyn_signal_counter = 1;
    tg_int_global.nest_asyn_signal = 1;
#if (CN_USE_TICKLESS_MODE)
    tick=__Djy_GetTimeBaseReload();
    user_systick(tick);
#else
    user_systick(1);
#endif
    tg_int_global.nest_asyn_signal = 0;
//    tg_int_global.en_asyn_signal = true;
    tg_int_global.en_asyn_signal_counter = 0;
//  if(g_ptEventReady != g_ptEventRunning)
//      __Djy_ScheduleAsynSignal();       //闁圭瑳鍡╂斀濞戞搩鍘介弻鍥礃閸涙壆娈堕幖杈炬嫹
    g_bScheduleEnable = true;
}

void HardExp_Init(void)
{
    __Exp_TableSet();
}

enum EN_BlackBoxAction Exp_MemManageFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

enum EN_BlackBoxAction Exp_HardFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

enum EN_BlackBoxAction Exp_DebugFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

enum EN_BlackBoxAction Exp_BusFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

enum EN_BlackBoxAction Exp_UsageFaultHandler(u32 fpu_used,u32 *core_info)
{
    return EN_BLACKBOX_DEAL_IGNORE;
}

bool_t  HardExp_Analysis(struct ExpThrowPara *parahead, u32 endian)
{
    return true;
}

bool_t  HardExp_Decoder(struct ExpThrowPara *parahead, u32 endian)
{
    return true;
}

bool_t HardExp_InfoDecoderInit(void)
{
    return true;
}

bool_t HardExp_Trap_c(uint32_t *regs)
{
    int i = 0, j = 0;
    int32_t inc = 0;
    uint32_t vec = 0;
    asm volatile(
        "mfcr    %0, psr \n"
        "lsri    %0, 16 \n"
        "sextb   %0 \n"
        :"=r"(vec):);

    printk("CPU Exception : %d", vec);
    printk("\n");

    for (; i < (16 + inc); i++, j++) {
        printk("r%d: %08x\t", j, regs[i]);

        if ((i % 5) == 4) {
            printk("\n");
        }
    }
    inc += 16;

    for (; i < (16 + inc); i++, j++) {
        printk("r%d: %08x\t", j, regs[i]);

        if ((i % 5) == 4) {
            printk("\n");
        }
    }

    printk("\n");
    printk("epsr: %8x\n", regs[i]);
    printk("epc : %8x\n", regs[i + 1]);

    while (1);
    return true;
}
