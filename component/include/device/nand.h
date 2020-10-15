//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

#ifndef __NAND_H__
#define __NAND_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//
//JEDEC��׼����
//
#define PAGE_READ_CMD_BYTE1                 (0x00)
#define PAGE_READ_CMD_BYTE2                 (0x30)
#define PAGE_PROGRAM_CMD_BYTE1              (0x80)
#define PAGE_PROGRAM_CMD_BYTE2              (0x10)
#define BLOCK_ERASE_CMD_BYTE1               (0x60)
#define BLOCK_ERASE_CMD_BYTE2               (0xD0)
#define ID_READ_CMD_BYTE                    (0x90)
#define STATUS_READ_CMD_BYTE                (0x70)
#define PARAMETER_PAGE_READ_CMD_BYTE        (0xEC)
#define RESET_CMD_BYTE                      (0xFF)

//
// @brief  NAND�豸��Ϣ
//
struct NandDescr{
    u16      Type;                      // �Ƿ�֧��ONFI
#define ONFI                            ((u16)0x0001)
#define RAW                             ((u16)0x0000)
    u16      Revision;                  // ONFI�汾��
    char     Manufacturer[13];          // оƬ������
    char     DeviceModel[21];           // оƬ�ͺ�
    u16      OOB_Size;                  // Out Of Band�ռ�ߴ�,�ֽ�Ϊ��λ
    u32      BytesPerPage;              // ҳ�а������ֽ���
    u32      PagesPerBlk;               // ���а�����ҳ��
    u32      BlksPerLUN;                // LUN�а����Ŀ���
    u8       LUNs;                      // LUN����
#if 0
    u8       SafeBlkNo;                 // ����ȷ�����õĿ��
#endif
    u8       BadMarkOffset;             // ��������spare�е�ƫ��,���鷽ʽ��λ
    u16      ReservedBlks;              // �����Ŀ������ӵ�0�鿪ʼ����
    u8       Controller;                // NAND���������Լ�����
#define NO_ECC_SUPPORTED                ((u8)0x0)
#define HW_ECC_SUPPORTED                ((u8)0x1)
#define SW_ECC_SUPPORTED                ((u8)0x2)
};


//
// @brief  ONFI
//
struct NandONFI
{
    //�޶���Ϣ��������
    s8 signature[4];             //ONFIǩ��
    s16 revision_num;            //֧�ֵ�ONFI�汾��
    s16 features_supp;           //����֧��
    s16 opt_cmd_supp;            //֧�ֿ�ѡ������
    s8 adv_cmd_supp;             //ONFI-JEDEC JTG��Ҫ�߼�����֧��
    s8 tar_cmd_supp;             //ѵ������֧��
    s16 ext_param_page_len;      //��չ����ҳ��
    s8 param_page_num;           //����ҳ��
    s8 feat_reserved[17];        //�޶���Ϣ���������б�����λ
    //��������Ϣ��
    s8 dev_manufacturer[12];         //�豸������
    s8 dev_model[20];            //�豸�ͺ�
    s8 manufacturer_id;          //JEDEC ������ID
    s16 data_code;               //���ݴ���
    s8 manuf_reserved[13];       //��������Ϣ���б�����λ
    //�ڴ�ṹ��
    s32 bytes_per_page;          //ÿҳ���ֽ���
    s16 spare_bytes_per_page;    //ÿҳ�ı����ֽ���
    s32 obsolete_bytes_per_partial_page;            //ÿ����ҳ���ֽ���(�ѷ���)
    s16 obsolete_spare_bytes_per_partial_page;      //ÿ����ҳ�ı����ֽ���(�ѷ���)
    s32 pages_per_block;          //ÿ���ҳ��
    s32 blocks_per_LUN;           //ÿ��LUN�Ŀ���
    s8 LUNs;                      //LUN��
    s8 address_cycles;            //��ַ��������0-3���е�ַ��������4-7���е�ַ������
    s8 bits_per_cell;             //ÿ����Ԫ�ı�����
    s16 bad_blk_max_per_LUN;      //ÿ��LUN����󻵿���
    s16 block_endurance;          //����;���
    s8 blocks_begin_valid;        //��Ŀ�꿪ʼʱ��֤��Ч��
    s16 valid_block_endurance;    //��֤��Ч��Ŀ�־���
    s8 programs_per_page;         //ÿҳ�ĳ�����
    s8 obsolete_Part_programs_attri;       //���ֱ������(�ѷ���)
    s8 ECC_corr_bits;             //ECC�ɾ���λ��
    s8 plane_address_bits;        //���ַλ��
    s8 multi_plane_oper_attri;    //�����������
    s8 EZ_nand_support;           //EZ nand֧��
    s8 memory_reserved[12];       //�ڴ�ṹ��ı���λ
    //����������
    s8 IO_capacitance_max;        //I/O���ŵ����������ֵ
    s16 SDR_timing_support;       //SDR��ʱģʽ֧��
    s16 obsolete_SDR_program_cache;     //֧��SDR���򻺴涨ʱģʽ(�ѷ���)
    s16 max_page_program_time;    //tPROG���ҳ�����ʱ��
    s16 max_block_erase_time;     //tBERS�������ʱ��
    s16 max_page_read_time;       //tR���ҳ��ȡʱ��
    s16 min_change_colu_set_time; //��С����������ʱ��
    s8 NV_DDR_timing_mode_supp;   //NV-DDR��ʱģʽ֧��
    s8 NV_DDR2_timing_mode_supp;   //NV-DDR2��ʱģʽ֧��
    s8 DDR_DDR2_features;          //NV-DDR - NV-DDR2����
    s16 CLK_input_capacitance;     //ʱ���������ŵ�����������ֵ
    s16 IO_capacitance_typical;    //I/O���ŵ�����������ֵ
    s16 input_capacitance_typical; //�������ŵ�����������ֵ
    s8 input_capacitance_max;      //�������ŵ����������ֵ
    s8 driver_strength_supp;       //��������֧��
    s16 multi_plane_page_read_time_max;     //������Ҳ��ʱ��
    s16 program_page_reg_clear;    //����ҳ�Ĵ��������ǿtADLֵ
    s16 typical_page_read_time;    //���͵�EZ NAND��ҳ���ȡʱ��
    s8 NV_DDR2_3_features;         //NV-DDR2/3����
    s8 NV_DDR2_3_warmup_cycles;    //NV-DDR2/3Ԥ������
    s16 NV_DDR3_timing_supp;       //NV-DDR3��ʱģʽ֧��
    s8 NV_DDR2_timing_supp;        //NV-DDR2��ʱģʽ֧��
    s8 electrical_reserved;        //���������飬����λ
    //��Ӧ�̵Ŀ�
    s16 vendor_specific_revision;  //��Ӧ���ض��汾��
    s8 vendor_specific[88];        //�ض��ڹ�Ӧ�̵�
    s16 integrity_CRC;             //����CRC
    //ʣ��Ĳ���ҳ
    s8 redundant1[256];
    s8 redundant2[256];

}__attribute__((packed));

//
//
//
struct NandSpecialOps{
    s32 (*ChkBlk)(u32 BlkNo);           // �����Ƿ��ѻ�
    s32 (*MrkBad)(u32 BlkNo);           // ��ǻ���
};
#ifdef __cplusplus
}
#endif

#endif //ifndef __NAND_H__
