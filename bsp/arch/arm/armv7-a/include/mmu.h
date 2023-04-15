#ifndef __MMU_H_
#define __MMU_H_

#ifdef __cplusplus
extern "C" {
#endif

extern u32 mmu_page_table[];

// cp15协处理器中的寄存器定义
// c0 - ID编码（只读）

// c1 - 控制寄存器
#define CN_BM_CP15_C1_MMU_EN 0x00000001 //使能MMU或者PU
#define CN_CP15_C1_MMU_EN 0
#define CN_CP15_C1_MEM_ALIGN_CHECK_EN 0x00000002 //使能内存访问时的地址对齐检查
#define CN_BO_CP15_C1_MEM_ALIGN_CHECK_EN 1
#define CN_BM_CP15_C1_CACHE_DATA_EN 0x00000004 //使能数据Cache（如果指令和数据Cache统一时，设置这个位会同时禁止指令和数据Cache）
#define CN_BO_CP15_C1_CACHE_DATA_EN 2
#define CN_BM_CP15_C1_WRITE_BUFFER_EN 0x00000008 //使能写缓冲
#define CN_BO_CP15_C1_WRITE_BUFFER_EN 3
#define CN_BM_CP15_C1_MEM_BIG_ENDIAN 0x00000080 //选择使用big-endian内存模式（仅对于两种端模式都支持的ARM才有效）
#define CN_BO_CP15_C1_MEM_BIG_ENDIAN 7
#define CN_BM_CP15_C1_SYS_PROTECTION 0x00000100 //在基于MMU的存储系统中，本控制位用作系统保护
#define CN_BO_CP15_C1_SYS_PROTECTION 8
#define CN_BM_CP15_C1_ROM_PROTECTION 0x00000200 //在基于MMU的存储系统中，本控制位用作ROM保护
#define CN_BO_CP15_C1_ROM_PROTECTION 9
#define CN_BM_CP15_C1_BR_PREDICTION_EN 0x00000800 //使能跳转预测功能（如果支持）
#define CN_BO_CP15_C1_BR_PREDICTION_E
#define CN_BM_CP15_C1_CACHE_INSTRUCTION_EN 0x00001000 //使能指令Cache
#define CN_BO_CP15_C1_CACHE_INSTRUCTION_EN 12
#define CN_BM_CP15_C1_EXCEPTIONS_LOCATION 0x00002000 //异常中断向量表的位置选择（0:0x0-0x1c;1:0xFFFF0000-0xFFFF001C）
#define CN_BO_CP15_C1_EXCEPTIONS_LOCATION 13
#define CN_BM_CP15_C1_RR_STRATEGY 0x00004000 //系统中Cache的淘汰算法（0:随机淘汰；1:Round-robin淘汰算法）
#define CN_BO_CP15_C1_RR_STRATEGY 14

// 一级描述符中各个位的定义（段定义）
#define CN_BM_FIRST_LEVEL_DES_AP 0x00000C00 //AP两个位
#define CN_BO_FIRST_LEVEL_DES_AP 10
#define CN_BM_FIRST_LEVEL_DES_DOMAIN 0x000001E0 //域
#define CN_BO_FIRST_LEVEL_DES_DOMAIN 5
#define CN_BM_FIRST_LEVEL_DES_CACHE 0x00000008 //C位
#define CN_BO_FIRST_LEVEL_DES_CACHE 3 //C位
#define CN_BM_FIRST_LEVEL_DES_BUFFER 0x00000004 //B位
#define CN_BO_FIRST_LEVEL_DES_BUFFER 2 //B位

////////////////////////////////////

//AP
#define AP_ROM                  0
#define AP_USER_NO              1
#define AP_USER_RO              2  //supervisor=RW, user=RO
#define AP_USER_RW              3  //supervisor=RW, user=RW

//Domain
#define DOMAIN_NO_ACCESS        0
#define DOMAIN_CHECK            1
#define DOMAIN_NO_CHECK         3

//Cache + Buffer
#define NCNB    0
#define NCB     1
#define CNB     2
#define CB      3

// 段属性
#define SECTION_ATTR_FAULT  (0)
#define SECTION_ATTR(ap, domain, cache_buffer) \
                        (ap << 10) | (domain << 5) | (1 << 4) | (cache_buffer << 2) | (1 << 1) | (0 << 0)

////////////////////////////////////

void MMU_MapSection(u32 *tt_base,u32 va_start,u32 pa_start,u32 sec_count,u32 attr);

void MMU_LockEntry();
void MMU_InvalidateTlbInstructionEntry(void);
void MMU_InvalidateTlbDatasEntry(void);
void MMU_InvalidateTlbInstructionsAll(void);
void MMU_InvalidateTlbDatasAll(void);
void MMU_InvalidateTlbsAll(void);
void MMU_SetSectionsBase(u32 *base);
void MMU_SetDomainAccess(u32 authorization);
void MMU_Disable(void);
void MMU_Enable(void);

#ifdef __cplusplus
}
#endif

#endif /* __MMU_H_ */
