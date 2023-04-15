#ifndef __MMU_H_
#define __MMU_H_

#ifdef __cplusplus
extern "C" {
#endif

extern u32 mmu_page_table[];

// cp15Э�������еļĴ�������
// c0 - ID���루ֻ����

// c1 - ���ƼĴ���
#define CN_BM_CP15_C1_MMU_EN 0x00000001 //ʹ��MMU����PU
#define CN_CP15_C1_MMU_EN 0
#define CN_CP15_C1_MEM_ALIGN_CHECK_EN 0x00000002 //ʹ���ڴ����ʱ�ĵ�ַ������
#define CN_BO_CP15_C1_MEM_ALIGN_CHECK_EN 1
#define CN_BM_CP15_C1_CACHE_DATA_EN 0x00000004 //ʹ������Cache�����ָ�������Cacheͳһʱ���������λ��ͬʱ��ָֹ�������Cache��
#define CN_BO_CP15_C1_CACHE_DATA_EN 2
#define CN_BM_CP15_C1_WRITE_BUFFER_EN 0x00000008 //ʹ��д����
#define CN_BO_CP15_C1_WRITE_BUFFER_EN 3
#define CN_BM_CP15_C1_MEM_BIG_ENDIAN 0x00000080 //ѡ��ʹ��big-endian�ڴ�ģʽ�����������ֶ�ģʽ��֧�ֵ�ARM����Ч��
#define CN_BO_CP15_C1_MEM_BIG_ENDIAN 7
#define CN_BM_CP15_C1_SYS_PROTECTION 0x00000100 //�ڻ���MMU�Ĵ洢ϵͳ�У�������λ����ϵͳ����
#define CN_BO_CP15_C1_SYS_PROTECTION 8
#define CN_BM_CP15_C1_ROM_PROTECTION 0x00000200 //�ڻ���MMU�Ĵ洢ϵͳ�У�������λ����ROM����
#define CN_BO_CP15_C1_ROM_PROTECTION 9
#define CN_BM_CP15_C1_BR_PREDICTION_EN 0x00000800 //ʹ����תԤ�⹦�ܣ����֧�֣�
#define CN_BO_CP15_C1_BR_PREDICTION_E
#define CN_BM_CP15_C1_CACHE_INSTRUCTION_EN 0x00001000 //ʹ��ָ��Cache
#define CN_BO_CP15_C1_CACHE_INSTRUCTION_EN 12
#define CN_BM_CP15_C1_EXCEPTIONS_LOCATION 0x00002000 //�쳣�ж��������λ��ѡ��0:0x0-0x1c;1:0xFFFF0000-0xFFFF001C��
#define CN_BO_CP15_C1_EXCEPTIONS_LOCATION 13
#define CN_BM_CP15_C1_RR_STRATEGY 0x00004000 //ϵͳ��Cache����̭�㷨��0:�����̭��1:Round-robin��̭�㷨��
#define CN_BO_CP15_C1_RR_STRATEGY 14

// һ���������и���λ�Ķ��壨�ζ��壩
#define CN_BM_FIRST_LEVEL_DES_AP 0x00000C00 //AP����λ
#define CN_BO_FIRST_LEVEL_DES_AP 10
#define CN_BM_FIRST_LEVEL_DES_DOMAIN 0x000001E0 //��
#define CN_BO_FIRST_LEVEL_DES_DOMAIN 5
#define CN_BM_FIRST_LEVEL_DES_CACHE 0x00000008 //Cλ
#define CN_BO_FIRST_LEVEL_DES_CACHE 3 //Cλ
#define CN_BM_FIRST_LEVEL_DES_BUFFER 0x00000004 //Bλ
#define CN_BO_FIRST_LEVEL_DES_BUFFER 2 //Bλ

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

// ������
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
