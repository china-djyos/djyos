//-----------------------------------------------------------------------------

// �ļ���     ��cpu_peri_BkSram.h
// ģ������: BackUp Sramͷ�ļ�
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 09/12.2014
// =============================================================================

#ifndef CPU_PERI_BACKUP_SRAM_H_
#define CPU_PERI_BACKUP_SRAM_H_

u32 BackUpSram_Read(u8 *rBuffer,u32 offset,u32 len);
u32 BackUpSram_Write(u8 *wBuffer,u32 offset,u32 len);
#endif /* CPU_PERI_BACKUP_SRAM_H_ */
