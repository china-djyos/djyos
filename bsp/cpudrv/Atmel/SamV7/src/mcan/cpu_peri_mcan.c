//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================

// �ļ���     ��cpu_peri_mcan.c
// ģ������: MCAN����
// ģ��汾: V1.00
// ������Ա: zhb
// ����ʱ��: 05/12.2016
// =============================================================================
/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include <assert.h>
#include "cpu_peri.h"
#include "ring.h"
#include "os.h"
#include "stdlib.h"
#include "shell.h"
#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "dbug.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    ModuleInstall_CAN(0);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip mcan"//CPU��can��������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                           //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                        //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                          //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                        //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                          //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"ring buffer and line buffer","shell","lock"//������������������������none����ʾ�������������
                                          //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                          //����������������������г����á�,���ָ�
//weakdependence:"none"                   //��������������������������none����ʾ�������������
                                          //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                          //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                          //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_MCAN == false )
//#warning  " cpu_onchip_mcan  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_MCAN    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
/*---------------------------------------------------------------------------
 *      Definitions
 *---------------------------------------------------------------------------*/
#define MAILBOX_ADDRESS(address) ( 0xFFFC & (address) )

#define CAN_CLK_FREQ_HZ               MCAN_PROG_CLK_FREQ_HZ

#define MCAN1_TSEG1                   ( MCAN1_PROP_SEG + MCAN1_PHASE_SEG1 )
#define MCAN1_TSEG2                   ( MCAN1_PHASE_SEG2 )
#define MCAN1_BRP                     ((uint32_t) (( (float) CAN_CLK_FREQ_HZ / \
                                    ((float)( MCAN1_TSEG1 + MCAN1_TSEG2 + 3 ) *\
                                    (float) MCAN1_BIT_RATE_BPS )) - 1 ))
#define MCAN1_SJW                     ( MCAN1_SYNC_JUMP - 1 )
#define MCAN1_FTSEG1                  ( MCAN1_FAST_PROP_SEG + MCAN1_FAST_PHASE_SEG1 )
#define MCAN1_FTSEG2                  ( MCAN1_FAST_PHASE_SEG2 )
#define MCAN1_FBRP                    ((uint32_t) (( (float) CAN_CLK_FREQ_HZ /\
                                    ((float)( MCAN1_FTSEG1 + MCAN1_FTSEG2 + 3 ) *\
                                    (float) MCAN1_FAST_BIT_RATE_BPS )) - 1 ))
#define MCAN1_FSJW                    ( MCAN1_FAST_SYNC_JUMP - 1 )

#define MCAN1_STD_FLTS_WRDS           (MCAN1_NMBR_STD_FLTS)
                                    /* 128 max filters */
#define MCAN1_EXT_FLTS_WRDS           (MCAN1_NMBR_EXT_FLTS * 2)
                                    /* 64 max filters */
#define MCAN1_RX_FIFO0_WRDS           (MCAN1_NMBR_RX_FIFO0_ELMTS * \
                                    ((MCAN1_RX_FIFO0_ELMT_SZ/4) + 2))
                                    /* 64 elements max */
#define MCAN1_RX_FIFO1_WRDS           (MCAN1_NMBR_RX_FIFO1_ELMTS *\
                                    ((MCAN1_RX_FIFO1_ELMT_SZ/4) + 2))
                                    /* 64 elements max */
#define MCAN1_RX_DED_BUFS_WRDS            (MCAN1_NMBR_RX_DED_BUF_ELMTS * \
                                    ((MCAN1_RX_BUF_ELMT_SZ/4) + 2))
                                    /* 64 elements max */
#define MCAN1_TX_EVT_FIFO_WRDS        (MCAN1_NMBR_TX_EVT_FIFO_ELMTS * 2)
/* 32 elements max */
#define MCAN1_TX_DED_BUF_WRDS         (MCAN1_NMBR_TX_DED_BUF_ELMTS * \
                                    ((MCAN1_TX_BUF_ELMT_SZ/4) + 2))
                                    /* 32 elements max */
#define MCAN1_TX_FIFO_Q_WRDS          (MCAN1_NMBR_TX_FIFO_Q_ELMTS * \
                                    ((MCAN1_TX_BUF_ELMT_SZ/4) + 2))
                                    /* 32 elements max */

/* validate CAN1 entries */
#if ( MCAN1_TSEG1 > 63 )
#error "Invalid CAN1 TSEG1"
#endif
#if (  MCAN1_TSEG2 > 15 )
#error "Invalid CAN1 TSEG2"
#endif
#if ( MCAN1_SJW > 15 )
#error "Invalid CAN1 SJW"
#endif
#if ( MCAN1_FTSEG1 > 15 )
#error "Invalid CAN1 FTSEG1"
#endif
#if (  MCAN1_FTSEG2 > 7 )
#error "Invalid CAN1 FTSEG2"
#endif
#if ( MCAN1_FSJW > 3 )
#error "Invalid CAN1 FSJW"
#endif

#if ( MCAN1_NMBR_STD_FLTS > 128 )
#error "Invalid CAN1 # of Standard Filters"
#endif
#if ( MCAN1_NMBR_EXT_FLTS > 64 )
#error "Invalid CAN1 # of Extended Filters"
#endif
#if ( MCAN1_NMBR_RX_FIFO0_ELMTS > 64 )
#error "Invalid CAN1 # RX FIFO 0 ELEMENTS"
#endif
#if ( MCAN1_NMBR_RX_FIFO1_ELMTS > 64 )
#error "Invalid CAN1 # RX FIFO 0 ELEMENTS"
#endif
#if ( MCAN1_NMBR_RX_DED_BUF_ELMTS > 64 )
#error "Invalid CAN1 # RX BUFFER ELEMENTS"
#endif
#if ( MCAN1_NMBR_TX_EVT_FIFO_ELMTS > 32 )
#error "Invalid CAN1 # TX EVENT FIFO ELEMENTS"
#endif
#if ( (MCAN1_NMBR_TX_DED_BUF_ELMTS + MCAN1_NMBR_TX_FIFO_Q_ELMTS)  > 32 )
#error "Invalid CAN1 # TX BUFFER ELEMENTS"
#endif

#if   ( 8 == MCAN1_RX_FIFO0_ELMT_SZ )
#define MCAN1_RX_FIFO0_DATA_SIZE  (0u)
#elif ( 12 == MCAN1_RX_FIFO0_ELMT_SZ )
#define MCAN1_RX_FIFO0_DATA_SIZE  (1u)
#elif ( 16 == MCAN1_RX_FIFO0_ELMT_SZ )
#define MCAN1_RX_FIFO0_DATA_SIZE  (2u)
#elif ( 20 == MCAN1_RX_FIFO0_ELMT_SZ )
#define MCAN1_RX_FIFO0_DATA_SIZE  (3u)
#elif ( 24 == MCAN1_RX_FIFO0_ELMT_SZ )
#define MCAN1_RX_FIFO0_DATA_SIZE  (4u)
#elif ( 32 == MCAN1_RX_FIFO0_ELMT_SZ )
#define MCAN1_RX_FIFO0_DATA_SIZE  (5u)
#elif ( 48 == MCAN1_RX_FIFO0_ELMT_SZ )
#define MCAN1_RX_FIFO0_DATA_SIZE  (6u)
#elif ( 64 == MCAN1_RX_FIFO0_ELMT_SZ )
#define MCAN1_RX_FIFO0_DATA_SIZE  (7u)
#else
#error "Invalid CAN1 RX FIFO0 ELEMENT SIZE"
#endif

#if   ( 8 == MCAN1_RX_FIFO1_ELMT_SZ )
#define MCAN1_RX_FIFO1_DATA_SIZE  (0u)
#elif ( 12 == MCAN1_RX_FIFO1_ELMT_SZ )
#define MCAN1_RX_FIFO1_DATA_SIZE  (1u)
#elif ( 16 == MCAN1_RX_FIFO1_ELMT_SZ )
#define MCAN1_RX_FIFO1_DATA_SIZE  (2u)
#elif ( 20 == MCAN1_RX_FIFO1_ELMT_SZ )
#define MCAN1_RX_FIFO1_DATA_SIZE  (3u)
#elif ( 24 == MCAN1_RX_FIFO1_ELMT_SZ )
#define MCAN1_RX_FIFO1_DATA_SIZE  (4u)
#elif ( 32 == MCAN1_RX_FIFO1_ELMT_SZ )
#define MCAN1_RX_FIFO1_DATA_SIZE  (5u)
#elif ( 48 == MCAN1_RX_FIFO1_ELMT_SZ )
#define MCAN1_RX_FIFO1_DATA_SIZE  (6u)
#elif ( 64 == MCAN1_RX_FIFO1_ELMT_SZ )
#define MCAN1_RX_FIFO1_DATA_SIZE  (7u)
#else
#error "Invalid CAN1 RX FIFO1 ELEMENT SIZE"
#endif

#if   ( 8 == MCAN1_RX_BUF_ELMT_SZ )
#define MCAN1_RX_BUF_DATA_SIZE  (0u)
#elif ( 12 == MCAN1_RX_BUF_ELMT_SZ )
#define MCAN1_RX_BUF_DATA_SIZE  (1u)
#elif ( 16 == MCAN1_RX_BUF_ELMT_SZ )
#define MCAN1_RX_BUF_DATA_SIZE  (2u)
#elif ( 20 == MCAN1_RX_BUF_ELMT_SZ )
#define MCAN1_RX_BUF_DATA_SIZE  (3u)
#elif ( 24 == MCAN1_RX_BUF_ELMT_SZ )
#define MCAN1_RX_BUF_DATA_SIZE  (4u)
#elif ( 32 == MCAN1_RX_BUF_ELMT_SZ )
#define MCAN1_RX_BUF_DATA_SIZE  (5u)
#elif ( 48 == MCAN1_RX_BUF_ELMT_SZ )
#define MCAN1_RX_BUF_DATA_SIZE  (6u)
#elif ( 64 == MCAN1_RX_BUF_ELMT_SZ )
#define MCAN1_RX_BUF_DATA_SIZE  (7u)
#else
#error "Invalid CAN1 RX BUFFER ELEMENT SIZE"
#endif

#if   ( 8 == MCAN1_TX_BUF_ELMT_SZ )
#define MCAN1_TX_BUF_DATA_SIZE  (0u)
#elif ( 12 == MCAN1_TX_BUF_ELMT_SZ )
#define MCAN1_TX_BUF_DATA_SIZE  (1u)
#elif ( 16 == MCAN1_TX_BUF_ELMT_SZ )
#define MCAN1_TX_BUF_DATA_SIZE  (2u)
#elif ( 20 == MCAN1_TX_BUF_ELMT_SZ )
#define MCAN1_TX_BUF_DATA_SIZE  (3u)
#elif ( 24 == MCAN1_TX_BUF_ELMT_SZ )
#define MCAN1_TX_BUF_DATA_SIZE  (4u)
#elif ( 32 == MCAN1_TX_BUF_ELMT_SZ )
#define MCAN1_TX_BUF_DATA_SIZE  (5u)
#elif ( 48 == MCAN1_TX_BUF_ELMT_SZ )
#define MCAN1_TX_BUF_DATA_SIZE  (6u)
#elif ( 64 == MCAN1_TX_BUF_ELMT_SZ )
#define MCAN1_TX_BUF_DATA_SIZE  (7u)
#else
#error "Invalid CAN1 TX BUFFER ELEMENT SIZE"
#endif

#define CAN_11_BIT_ID_MASK                 (0x7FF)
#define CAN_29_BIT_ID_MASK                 (0x1FFFFFFF)
#define ELMT_SIZE_MASK                (0x1F)
/* max element size is 18 words, fits in 5 bits */
#define BUFFER_XTD_MASK               (0x40000000)
#define BUFFER_EXT_ID_MASK            (0x1FFFFFFF)
#define BUFFER_STD_ID_MASK            (0x1FFC0000)
#define BUFFER_DLC_MASK               (0x000F0000)
#define BUFFER_RXTS_MASK              (0x0000FFFF)

#define STD_FILT_SFT_MASK             (3 << 30)
#define STD_FILT_SFT_RANGE            (0 << 30)
#define STD_FILT_SFT_DUAL             (1 << 30)
#define STD_FILT_SFT_CLASSIC          (2 << 30)
#define STD_FILT_SFEC_MASK            (7 << 27)
#define STD_FILT_SFEC_DISABLE         (0 << 27)
#define STD_FILT_SFEC_FIFO0           (1 << 27)
#define STD_FILT_SFEC_FIFO1           (2 << 27)
#define STD_FILT_SFEC_REJECT          (3 << 27)
#define STD_FILT_SFEC_PRIORITY        (4 << 27)
#define STD_FILT_SFEC_PRIORITY_FIFO0  (5 << 27)
#define STD_FILT_SFEC_PRIORITY_FIFO1  (6 << 27)
#define STD_FILT_SFEC_BUFFER          (7 << 27)
#define STD_FILT_SFID1_MASK           (0x03FF << 16)
#define STD_FILT_SFID2_MASK           (0x3FF << 0)
#define STD_FILT_SFID2_RX_BUFFER      (0 << 9)
#define STD_FILT_SFID2_DEBUG_A        (1 << 9)
#define STD_FILT_SFID2_DEBUG_B        (2 << 9)
#define STD_FILT_SFID2_DEBUG_C        (3 << 9)
#define STD_FILT_SFID2_BUFFER(nmbr)   (nmbr & 0x3F)

#define EXT_FILT_EFEC_MASK            (7 << 29)
#define EXT_FILT_EFEC_DISABLE         (0 << 29)
#define EXT_FILT_EFEC_FIFO0           (1 << 29)
#define EXT_FILT_EFEC_FIFO1           (2 << 29)
#define EXT_FILT_EFEC_REJECT          (3 << 29)
#define EXT_FILT_EFEC_PRIORITY        (4 << 29)
#define EXT_FILT_EFEC_PRIORITY_FIFO0  (5 << 29)
#define EXT_FILT_EFEC_PRIORITY_FIFO1  (6 << 29)
#define EXT_FILT_EFEC_BUFFER          (7 << 29)
#define EXT_FILT_EFID1_MASK           (0x1FFFFFFF)
#define EXT_FILT_EFT_MASK             (3 << 30)
#define EXT_FILT_EFT_RANGE            (0 << 30)
#define EXT_FILT_EFT_DUAL             (1 << 30)
#define EXT_FILT_EFT_CLASSIC          (2 << 30)
#define EXT_FILT_EFT_RANGE_NO_XIDAM   (3 << 30)
#define EXT_FILT_EFID2_MASK           (0x1FFFFFFF)
#define EXT_FILT_EFID2_RX_BUFFER      (0 << 9)
#define EXT_FILT_EFID2_DEBUG_A        (1 << 9)
#define EXT_FILT_EFID2_DEBUG_B        (2 << 9)
#define EXT_FILT_EFID2_DEBUG_C        (3 << 9)
#define EXT_FILT_EFID2_BUFFER(nmbr)   (nmbr & 0x3F)

#define CN_CAN_RX_BUF_SIZE                4096
#define CN_CAN_DEV_MONITOR_TIME           3*1000*1000


typedef enum
{
    MCAN_INTR_LINE_0 = 0,
    MCAN_INTR_LINE_1 = 1
} MCan_IntrLineType;

typedef enum
{
    CAN_DLC_0 = 0,
    CAN_DLC_1 = 1,
    CAN_DLC_2 = 2,
    CAN_DLC_3 = 3,
    CAN_DLC_4 = 4,
    CAN_DLC_5 = 5,
    CAN_DLC_6 = 6,
    CAN_DLC_7 = 7,
    CAN_DLC_8 = 8,
    CAN_DLC_12 = 9,
    CAN_DLC_16 = 10,
    CAN_DLC_20 = 11,
    CAN_DLC_24 = 12,
    CAN_DLC_32 = 13,
    CAN_DLC_48 = 14,
    CAN_DLC_64 = 15
} MCan_DlcType;

typedef enum
{
    MCAN_STD_ID = 0,
    MCAN_EXT_ID = 1
} MCan_IdType;


typedef enum
{
    MCAN_FIFO_0 = 0,
    MCAN_FIFO_1 = 1
} MCan_FifoType;
/*---------------------------------------------------------------------------
 *      Internal variables
 *---------------------------------------------------------------------------*/
static const Pin pinsMcan1[] =  {PIN_MCAN1_TXD, PIN_MCAN1_RXD };
static uint32_t can1MsgRam[MCAN1_STD_FLTS_WRDS +
                           MCAN1_EXT_FLTS_WRDS +
                           MCAN1_RX_FIFO0_WRDS +
                           MCAN1_RX_FIFO1_WRDS +
                           MCAN1_RX_DED_BUFS_WRDS +
                           MCAN1_TX_EVT_FIFO_WRDS +
                           MCAN1_TX_DED_BUF_WRDS +
                           MCAN1_TX_FIFO_Q_WRDS];

const MCan_ConfigType mcan1Config =
{
    MCAN1,
    MCAN_BTP_BRP(MCAN1_BRP) | MCAN_BTP_TSEG1(MCAN1_TSEG1) |
    MCAN_BTP_TSEG2(MCAN1_TSEG2) | MCAN_BTP_SJW(MCAN1_SJW),
    MCAN_FBTP_FBRP(MCAN1_FBRP) | MCAN_FBTP_FTSEG1(MCAN1_FTSEG1) |
    MCAN_FBTP_FTSEG2(MCAN1_FTSEG2) | MCAN_FBTP_FSJW(MCAN1_FSJW),
    MCAN1_NMBR_STD_FLTS,
    MCAN1_NMBR_EXT_FLTS,
    MCAN1_NMBR_RX_FIFO0_ELMTS,
    MCAN1_NMBR_RX_FIFO1_ELMTS,
    MCAN1_NMBR_RX_DED_BUF_ELMTS,
    MCAN1_NMBR_TX_EVT_FIFO_ELMTS,
    MCAN1_NMBR_TX_DED_BUF_ELMTS,
    MCAN1_NMBR_TX_FIFO_Q_ELMTS,
    (MCAN1_RX_FIFO0_DATA_SIZE << 29) | ((MCAN1_RX_FIFO0_ELMT_SZ/4)+2),
    /* element size in WORDS */
    (MCAN1_RX_FIFO1_DATA_SIZE << 29) | ((MCAN1_RX_FIFO1_ELMT_SZ/4)+2),
    /* element size in WORDS */
    (MCAN1_RX_BUF_DATA_SIZE << 29) | ((MCAN1_RX_BUF_ELMT_SZ/4)+2),
    /* element size in WORDS */
    (MCAN1_TX_BUF_DATA_SIZE << 29) | ((MCAN1_TX_BUF_ELMT_SZ/4)+2),
    /* element size in WORDS */
    {
        &can1MsgRam[0],
        &can1MsgRam[MCAN1_STD_FLTS_WRDS],
        &can1MsgRam[MCAN1_STD_FLTS_WRDS + MCAN1_EXT_FLTS_WRDS],
        &can1MsgRam[MCAN1_STD_FLTS_WRDS + MCAN1_EXT_FLTS_WRDS + MCAN1_RX_FIFO0_WRDS],
        &can1MsgRam[MCAN1_STD_FLTS_WRDS + MCAN1_EXT_FLTS_WRDS + MCAN1_RX_FIFO0_WRDS
        + MCAN1_RX_FIFO1_WRDS],
        &can1MsgRam[MCAN1_STD_FLTS_WRDS + MCAN1_EXT_FLTS_WRDS + MCAN1_RX_FIFO0_WRDS
        + MCAN1_RX_FIFO1_WRDS + MCAN1_RX_DED_BUFS_WRDS],
        &can1MsgRam[MCAN1_STD_FLTS_WRDS + MCAN1_EXT_FLTS_WRDS + MCAN1_RX_FIFO0_WRDS
        + MCAN1_RX_FIFO1_WRDS + MCAN1_RX_DED_BUFS_WRDS + MCAN1_TX_EVT_FIFO_WRDS],
        &can1MsgRam[MCAN1_STD_FLTS_WRDS + MCAN1_EXT_FLTS_WRDS + MCAN1_RX_FIFO0_WRDS
        + MCAN1_RX_FIFO1_WRDS + MCAN1_RX_DED_BUFS_WRDS + MCAN1_TX_EVT_FIFO_WRDS
        + MCAN1_TX_DED_BUF_WRDS]
    },
};

static struct RingBuf *gs_RxRingBuf;
static Mailbox8Type   gs_rxMailbox;

static u64 gs_u64AppSndCnt=0,gs_u64HardSndCnt=0,gs_u64HardRcvCnt=0,gs_u64AppRcvCnt=0;
static u64 gs_u64BusOffCnt=0,gs_u64CrcErrCnt=0,gs_u64BitErrCnt=0,gs_u64FoErrCnt=0;
static u64 gs_u64StErrCnt=0,gs_u64ACKErrCnt=0;
static u64 gs_TxErrRstCnt=0,gs_RxErrRstCnt=0;

//���ݰ�����������Ч���ݲ�Ϊ8���ֽ�
static u64 gs_u64RcvPkgBadCnt=0,gs_u64SndPkgBadCnt=0;
static uint8_t CAN_MonitorStack[0x400];
#define CN_DEBUG_CAN_CNT    10
static uint8_t gs_SndPkg[13*CN_DEBUG_CAN_CNT];
static uint8_t gs_RcvPkg[13*CN_DEBUG_CAN_CNT];
static uint8_t gs_SndDebugCnt=0,gs_RcvDebugCnt=0;
static bool_t gs_CanDebugFlag=false;

/*---------------------------------------------------------------------------
 * ��������
 *---------------------------------------------------------------------------*/
void MCAN1_Line1_ISR_Handler(void);
void MCAN_Init(const MCan_ConfigType * mcanConfig );
void MCAN_InitTxQueue(const MCan_ConfigType * mcanConfig );
void MCAN_Enable(const MCan_ConfigType * mcanConfig );
void MCAN_RequestIso11898_1(const MCan_ConfigType * mcanConfig );
void MCAN_IEnableMessageStoredToRxDedBuffer(const MCan_ConfigType * mcanConfig,\
    MCan_IntrLineType line );
uint8_t  * MCAN_ConfigTxDedBuffer(const MCan_ConfigType * mcanConfig,uint8_t buffer,\
    uint32_t id,MCan_IdType idType,MCan_DlcType dlc );
void MCAN_SendTxDedBuffer(const MCan_ConfigType * mcanConfig, uint8_t buffer );
uint32_t MCAN_AddToTxFifoQ(const MCan_ConfigType * mcanConfig, uint32_t id, \
        MCan_IdType idType, MCan_DlcType dlc, uint8_t * data );
uint8_t MCAN_IsBufferTxd(const MCan_ConfigType * mcanConfig,uint8_t buffer );
void MCAN_ConfigRxBufferFilter(const MCan_ConfigType * mcanConfig,uint32_t buffer,\
    uint32_t filter,uint32_t id,MCan_IdType idType);
void MCAN_ConfigRxClassicFilter(const MCan_ConfigType * mcanConfig,\
        MCan_FifoType fifo,uint8_t filter,uint32_t id,MCan_IdType idType,\
        uint32_t mask );
uint8_t MCAN_IsNewDataInRxDedBuffer(const MCan_ConfigType * mcanConfig,\
    uint8_t buffer );
void MCAN_GetRxDedBuffer(const MCan_ConfigType * mcanConfig,uint8_t buffer,\
    Mailbox64Type * pRxMailbox );
uint32_t MCAN_GetRxFifoBuffer(const MCan_ConfigType * mcanConfig,\
        MCan_FifoType fifo,Mailbox64Type * pRxMailbox );
static void  __MCANIntInit(void);

void canrst(void);
void canstat(void);
void canreg(void);
void canpkg(void);




/*******************************************************************************
����:CAN����������shellģ��
����:��.
����ֵ:1��
*********************************************************************************/
ptu32_t MCAN_Shell_Module_Install(void)
{
    return 1;
}

/*******************************************************************************
����:��λCAN��������
����:�ޡ�
���:�ޡ�
*********************************************************************************/
//void Sh_CAN_Reset(void)
bool_t canrst(void)
{
    MCAN_Init(&mcan1Config);
    return true;
}


#define CN_PRINT_PREFIX  "   "
/*******************************************************************************
����:��ȡCAN����ͨ��ͳ������
����:�ޡ�
���:�ޡ�
*********************************************************************************/
//void Sh_CAN_Stat(void)
bool_t canstat(void)
{
   uint32_t data[2];//used to print the s64 type
   debug_printf("mcan","CAN Stat:\r\n");
   memcpy(data,&gs_u64AppSndCnt,sizeof(data));
   debug_printf("mcan","%s APP Snd Cnt      :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
   memcpy(data,&gs_u64HardSndCnt,sizeof(data));
   debug_printf("mcan","%s Hard Snd Cnt     :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
   memcpy(data,&gs_u64HardRcvCnt,sizeof(data));
   debug_printf("mcan","%s Hard Rcv Cnt     :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
   memcpy(data,&gs_u64AppRcvCnt,sizeof(data));
   debug_printf("mcan","%s APP Rcv Cnt      :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
   memcpy(data,&gs_u64SndPkgBadCnt,sizeof(data));
   debug_printf("mcan","%s Snd Bad Cnt      :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
   memcpy(data,&gs_u64RcvPkgBadCnt,sizeof(data));
   debug_printf("mcan","%s Rcv Bad Cnt      :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
   memcpy(data,&gs_TxErrRstCnt,sizeof(data));
   debug_printf("mcan","%s Snd Err Rst Cnt  :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
   memcpy(data,&gs_RxErrRstCnt,sizeof(data));
   debug_printf("mcan","%s Rcv Err Rst Cnt  :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
   memcpy(data,&gs_u64BusOffCnt,sizeof(data));
   debug_printf("mcan","%s Bus Off Cnt      :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
   memcpy(data,&gs_u64CrcErrCnt,sizeof(data));
   debug_printf("mcan","%s CRC Err Cnt      :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
   memcpy(data,&gs_u64BitErrCnt,sizeof(data));
   debug_printf("mcan","%s Bit Err Cnt      :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
   memcpy(data,&gs_u64FoErrCnt,sizeof(data));
   debug_printf("mcan","%s Format Err Cnt   :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
   memcpy(data,&gs_u64StErrCnt,sizeof(data));
   debug_printf("mcan","%s Stuff Err Cnt    :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
   memcpy(data,&gs_u64ACKErrCnt,sizeof(data));
   debug_printf("mcan","%s ACK Err Cnt      :0x%08x 0x%08x \r\n",CN_PRINT_PREFIX,data[1],data[0]);
   debug_printf("mcan","The End.\r\n");

   return true;
}

/*******************************************************************************
����:��ȡCAN�ɶ��Ĵ�����ֵ��
����:��.
���:�ޡ�
*********************************************************************************/
//void Sh_Read_CAN_Reg(void)
bool_t canreg(void)
{
      vu32 Reg;
      debug_printf("mcan","CAN Reg:\r\n");
      Reg=MCAN1->MCAN_RWD;
      debug_printf("mcan","%s MCAN_RWD     :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_CCCR;
      debug_printf("mcan","%s MCAN_CCCR    :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_BTP;
      debug_printf("mcan","%s MCAN_BTP     :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_TSCC;
      debug_printf("mcan","%s MCAN_TSCC    :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_TSCV;
      debug_printf("mcan","%s MCAN_TSCV    :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_TOCC;
      debug_printf("mcan","%s MCAN_TOCC    :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_TOCV;
      debug_printf("mcan","%s MCAN_TOCV    :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_ECR;
      debug_printf("mcan","%s MCAN_ECR     :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_PSR;
      debug_printf("mcan","%s MCAN_PSR     :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_IR;
      debug_printf("mcan","%s MCAN_IR      :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_IE;
      debug_printf("mcan","%s MCAN_IE      :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_ILS;
      debug_printf("mcan","%s MCAN_ILS     :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_ILE;
      debug_printf("mcan","%s MCAN_ILE     :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_GFC;
      debug_printf("mcan","%s MCAN_GFC     :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_SIDFC;
      debug_printf("%s MCAN_SIDFC   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_XIDFC;
      debug_printf("mcan","%s MCAN_XIDFC   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_XIDAM;
      debug_printf("mcan","%s MCAN_XIDAM   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_HPMS;
      debug_printf("mcan","%s MCAN_HPMS    :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_NDAT1;
      debug_printf("mcan","%s MCAN_NDAT1   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_NDAT2;
      debug_printf("mcan","%s MCAN_NDAT2   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_RXF0C;
      debug_printf("mcan","%s MCAN_RXF0C   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_RXF0S;
      debug_printf("mcan","%s MCAN_RXF0S   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_RXF0A;
      debug_printf("mcan","%s MCAN_RXF0A   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_RXBC;
      debug_printf("%s MCAN_RXBC    :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_RXF1C;
      debug_printf("mcan","%s MCAN_RXF1C   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_RXF1S;
      debug_printf("mcan","%s MCAN_RXF1S   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_RXF1A;
      debug_printf("mcan","%s MCAN_RXF1A   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_RXESC;
      debug_printf("mcan","%s MCAN_RXESC   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_TXBC;
      debug_printf("mcan","%s MCAN_TXBC    :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_TXFQS;
      debug_printf("mcan","%s MCAN_TXFQS   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_TXESC;
      debug_printf("mcan","%s MCAN_TXESC   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_TXBRP;
      debug_printf("mcan","%s MCAN_TXBRP   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_TXBAR;
      debug_printf("mcan","%s MCAN_TXBC    :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_TXBCR;
      debug_printf("mcan","%s MCAN_TXFQS   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_TXBTO;
      debug_printf("mcan","%s MCAN_TXESC   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_TXBCF;
      debug_printf("mcan","%s MCAN_TXBRP   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_TXBTIE;
      debug_printf("mcan","%s MCAN_TXBTIE  :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_TXBCIE;
      debug_printf("mcan","%s MCAN_TXBCIE  :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_TXEFC;
      debug_printf("mcan","%s MCAN_TXEFC   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_TXEFS;
      debug_printf("mcan","%s MCAN_TXEFS   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      Reg=MCAN1->MCAN_TXEFA;
      debug_printf("mcan","%s MCAN_TXEFA   :0x%08x\r\n",CN_PRINT_PREFIX,Reg);
      debug_printf("mcan","The End.\r\n");
      return true;
}

/*******************************************************************************
����:��ȡCAN�ɶ��Ĵ�����ֵ��
����:��.
���:�ޡ�
*********************************************************************************/
//void Sh_CAN_Pkg(void)
bool_t canpkg(void)
{
    uint8_t i,j;
    gs_CanDebugFlag=true;
    DJY_EventDelay(3*1000*mS);
    debug_printf("mcan","CAN Pkg Snd/Rcv:\r\n");
    debug_printf("mcan","%s Snd:\r\n",CN_PRINT_PREFIX);
    for(i=0;i<CN_DEBUG_CAN_CNT;i++)
    {
        debug_printf("mcan","%s ID:%d ",CN_PRINT_PREFIX,i);
       for(j=0;j<13;j++)
       {
           debug_printf("mcan","0x%x ",gs_SndPkg[13*i+j]);
       }
       debug_printf("mcan","\r\n");
    }
    debug_printf("mcan","%s Rcv:\r\n",CN_PRINT_PREFIX);
    for(i=0;i<CN_DEBUG_CAN_CNT;i++)
    {
        debug_printf("mcan","%s ID:%d ",CN_PRINT_PREFIX,i);
       for(j=0;j<13;j++)
       {
           debug_printf("mcan","0x%x ",gs_RcvPkg[13*i+j]);
       }
       printf("\r\n");
    }
    gs_CanDebugFlag=false;
    gs_SndDebugCnt=0;
    gs_RcvDebugCnt=0;
    memset(gs_SndPkg,0,13*CN_DEBUG_CAN_CNT);
    memset(gs_RcvPkg,0,13*CN_DEBUG_CAN_CNT);
    debug_printf("mcan","The End.\r\n");

    return true;
}
//----��ʼ��CAN���ò���---------------------------------------------------------
//����: ��ʼ��CAN���ò���
//����: pCanPara,CAN���������ò����ṹ��ָ��
//����: �ɹ���ʼ���򷵻�true,�����򷵻�false.
//-----------------------------------------------------------------------------

static void  __MCANIntInit(void)
{
    Int_Register(CN_INT_LINE_MCAN1);
    Int_SetClearType(CN_INT_LINE_MCAN1,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(CN_INT_LINE_MCAN1,MCAN1_Line1_ISR_Handler);
    Int_SettoAsynSignal(CN_INT_LINE_MCAN1);
    Int_ClearLine(CN_INT_LINE_MCAN1);
    Int_RestoreAsynLine(CN_INT_LINE_MCAN1);

    Int_Register(CN_INT_LINE_MCAN1_LINE1);
    Int_SetClearType(CN_INT_LINE_MCAN1_LINE1,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(CN_INT_LINE_MCAN1_LINE1,MCAN1_Line1_ISR_Handler);
    Int_SettoAsynSignal(CN_INT_LINE_MCAN1_LINE1);
    Int_ClearLine(CN_INT_LINE_MCAN1_LINE1);
    Int_RestoreAsynLine(CN_INT_LINE_MCAN1_LINE1);
}


 /**
 * \brief Initializes the MCAN hardware for giving peripheral.
 * Default: Mixed mode TX Buffer + FIFO.
 *
 * \param mcanConfig  Pointer to a MCAN instance.
 */
void MCAN_Init( const MCan_ConfigType * mcanConfig )
{
    Mcan      * mcan = mcanConfig->pMCan;
    uint32_t    regVal32;
    uint32_t  * pMsgRam;
    uint32_t    cntr;

    /* Both MCAN controllers use programmable clock 5 to derive bit rate */
    // select MCK divided by 1 as programmable clock 5 output
    PMC->PMC_PCK[5] = PMC_PCK_PRES(MCAN_PROG_CLK_PRESCALER - 1) | MCAN_PROG_CLK_SELECT;
    PMC->PMC_SCER = PMC_SCER_PCK5;

    PIO_Configure(pinsMcan1, PIO_LISTSIZE(pinsMcan1));
    // Enable MCAN peripheral clock
    PMC_EnablePeripheral( ID_MCAN1 );
    // Configure Message RAM Base Address
    regVal32 = MATRIX->CCFG_SYSIO & 0x0000FFFF;
    MATRIX->CCFG_SYSIO = regVal32 | ( (uint32_t) mcanConfig->msgRam.pStdFilts & 0xFFFF0000 );

    /* Indicates Initialization state */
    mcan->MCAN_CCCR = MCAN_CCCR_INIT_ENABLED;
    do { regVal32 = mcan->MCAN_CCCR; }
        while(0u == (regVal32 & MCAN_CCCR_INIT_ENABLED));

    /* Enable writing to configuration registers */
    mcan->MCAN_CCCR = MCAN_CCCR_INIT_ENABLED | MCAN_CCCR_CCE_CONFIGURABLE;

    /* Global Filter Configuration: Reject remote frames, accept non-matching frames */
    /*Message stored in Receive FIFO 0*/
    mcan->MCAN_GFC = MCAN_GFC_RRFE_REJECT | MCAN_GFC_RRFS_REJECT
            | MCAN_GFC_ANFE(0) | MCAN_GFC_ANFS(0);

    // Extended ID Filter AND mask
//  mcan->MCAN_XIDAM = 0x1FFFFFFF;

    mcan->MCAN_XIDAM = 0x0;    //todo

    /* Interrupt configuration - leave initialization with all interrupts off */
    // Disable all interrupts
    mcan->MCAN_IE =  0;
    mcan->MCAN_TXBTIE = 0x00000000;
    // All interrupts directed to Line 0
    mcan->MCAN_ILS =  0x00000000;
    // Disable both interrupt LINE 0 & LINE 1
    mcan->MCAN_ILE = 0x00;
    // Clear all interrupt flags
    mcan->MCAN_IR = 0xFFCFFFFF;

    /* Configure CAN bit timing */
    mcan->MCAN_BTP = mcanConfig->bitTiming;
    mcan->MCAN_FBTP = mcanConfig->fastBitTiming;

    /* Configure message RAM starting addresses & sizes */
    mcan->MCAN_SIDFC = MAILBOX_ADDRESS( (uint32_t) mcanConfig->msgRam.pStdFilts )
            | MCAN_SIDFC_LSS(mcanConfig->nmbrStdFilts);
    mcan->MCAN_XIDFC = MAILBOX_ADDRESS( (uint32_t) mcanConfig->msgRam.pExtFilts )
            | MCAN_XIDFC_LSE(mcanConfig->nmbrExtFilts);
    mcan->MCAN_RXF0C = MAILBOX_ADDRESS( (uint32_t) mcanConfig->msgRam.pRxFifo0 )
            | MCAN_RXF0C_F0S(mcanConfig->nmbrFifo0Elmts);
    // watermark interrupt off, blocking mode
    mcan->MCAN_RXF1C = MAILBOX_ADDRESS( (uint32_t) mcanConfig->msgRam.pRxFifo1 )
            | MCAN_RXF1C_F1S(mcanConfig->nmbrFifo1Elmts);
    // watermark interrupt off, blocking mode
    mcan->MCAN_RXBC = MAILBOX_ADDRESS( (uint32_t) mcanConfig->msgRam.pRxDedBuf );
    mcan->MCAN_TXEFC = MAILBOX_ADDRESS( (uint32_t) mcanConfig->msgRam.pTxEvtFifo )
            | MCAN_TXEFC_EFS(mcanConfig->nmbrTxEvtFifoElmts);
    // watermark interrupt off
    mcan->MCAN_TXBC = MAILBOX_ADDRESS( (uint32_t) mcanConfig->msgRam.pTxDedBuf )
            | MCAN_TXBC_NDTB(mcanConfig->nmbrTxDedBufElmts)
            | MCAN_TXBC_TFQS(mcanConfig->nmbrTxFifoQElmts);
    mcan->MCAN_RXESC = ((mcanConfig->rxBufElmtSize >> (29-MCAN_RXESC_RBDS_Pos)) &
                        MCAN_RXESC_RBDS_Msk) |
                        ((mcanConfig->rxFifo1ElmtSize >> (29-MCAN_RXESC_F1DS_Pos)) &
                        MCAN_RXESC_F1DS_Msk) |
                        ((mcanConfig->rxFifo0ElmtSize >> (29-MCAN_RXESC_F0DS_Pos)) &
                       MCAN_RXESC_F0DS_Msk);
    mcan->MCAN_TXESC = ((mcanConfig->txBufElmtSize >> (29-MCAN_TXESC_TBDS_Pos)) &
                MCAN_TXESC_TBDS_Msk);

    /* Configure Message Filters */
    // ...Disable all standard filters
    pMsgRam = mcanConfig->msgRam.pStdFilts;
    cntr = mcanConfig->nmbrStdFilts;
    while ( cntr > 0 ) {
      *pMsgRam++ = STD_FILT_SFEC_DISABLE;
      cntr--;
    }
    // ...Disable all extended filters
    pMsgRam = mcanConfig->msgRam.pExtFilts;
    cntr = mcanConfig->nmbrExtFilts;
    while ( cntr > 0 ) {
      *pMsgRam = EXT_FILT_EFEC_DISABLE;
      pMsgRam = pMsgRam + 2;
      cntr--;
    }

    mcan->MCAN_NDAT1 = 0xFFFFFFFF;  // clear new (rx) data flags
    mcan->MCAN_NDAT2 = 0xFFFFFFFF;  // clear new (rx) data flags

    regVal32 =  mcan->MCAN_CCCR & ~(MCAN_CCCR_CME_Msk | MCAN_CCCR_CMR_Msk);
    mcan->MCAN_CCCR = regVal32 | MCAN_CCCR_CME_ISO11898_1;
    mcan->MCAN_CCCR = regVal32 | (MCAN_CCCR_CMR_ISO11898_1 | MCAN_CCCR_CME_ISO11898_1);

    //�ж�����
    __MCANIntInit();

    MCAN_Enable( &mcan1Config );

    MCAN_IEnableMessageStoredToRxDedBuffer( &mcan1Config, MCAN_INTR_LINE_1 );

    __DSB();
    __ISB();
}

/**
 * \brief Initializes MCAN queue for TX
 * INIT must be set - so this should be called between MCAN_Init() and
 * MCAN_Enable()
 * \param mcanConfig  Pointer to a MCAN instance.
 */
void MCAN_InitTxQueue( const MCan_ConfigType * mcanConfig )
{
    Mcan * mcan = mcanConfig->pMCan;
    mcan->MCAN_TXBC |= MCAN_TXBC_TFQM;
}

/**
 * \brief Enable MCAN peripheral.
 * INIT must be set - so this should be called between MCAN_Init()
 * \param mcanConfig  Pointer to a MCAN instance.
 */
void MCAN_Enable( const MCan_ConfigType * mcanConfig )
{
    Mcan * mcan = mcanConfig->pMCan;
    mcan->MCAN_CCCR &= ~MCAN_CCCR_INIT_ENABLED;
}

/**
 * \brief Requests switch to Iso11898-1 (standard / classic) mode (tx & rx
 * payloads up to 8 bytes).
 * \param mcanConfig  Pointer to a MCAN instance.
 */
void MCAN_RequestIso11898_1( const MCan_ConfigType * mcanConfig )
{
    Mcan     * mcan = mcanConfig->pMCan;
    uint32_t   regVal32;

    regVal32 =  mcan->MCAN_CCCR & ~MCAN_CCCR_CMR_Msk;
    mcan->MCAN_CCCR = regVal32 | MCAN_CCCR_CMR_ISO11898_1;
    while ( (mcan->MCAN_CCCR & ( MCAN_CCCR_FDBS | MCAN_CCCR_FDO )) != 0 )
        { /* wait */ }
}

/**
 * \brief Enable message line and message stored to Dedicated Receive Buffer
 * Interrupt Line.
 * \param mcanConfig  Pointer to a MCAN instance.
 * \param line  Message line.
 */
void MCAN_IEnableMessageStoredToRxDedBuffer( const MCan_ConfigType * mcanConfig,
        MCan_IntrLineType line )
{
    Mcan * mcan = mcanConfig->pMCan;
    if ( line == MCAN_INTR_LINE_0 ) {
        mcan->MCAN_ILS &= ~MCAN_ILS_DRXL;
        mcan->MCAN_ILE |= MCAN_ILE_EINT0;
    } else  {
    // Interrupt Line 1
        //mcan->MCAN_ILS |= MCAN_ILS_DRXL;
        mcan->MCAN_ILS |= MCAN_ILS_DRXL|MCAN_ILS_RF0NL;  //todo
        mcan->MCAN_ILE |= MCAN_ILE_EINT1;
    }
    mcan->MCAN_IR = MCAN_IR_DRX|MCAN_IR_RF0N;  // clear previous flag
    mcan->MCAN_IE |= MCAN_IE_DRXE|MCAN_IE_RF0NE;  // enable it
}

/**
 * \brief Configures a Dedicated TX Buffer.
 * \param mcanConfig  Pointer to a MCAN instance.
 * \param buffer  Pointer to buffer.
 * \param id  Message ID.
 * \param idType  Type of ID
 * \param dlc  Type of dlc.
 */
uint8_t  * MCAN_ConfigTxDedBuffer( const MCan_ConfigType * mcanConfig,
        uint8_t buffer, uint32_t id, MCan_IdType idType, MCan_DlcType dlc )
{
     Mcan * mcan = mcanConfig->pMCan;
     uint32_t * pThisTxBuf = 0;

     if ( buffer < mcanConfig->nmbrTxDedBufElmts ) {
        pThisTxBuf = mcanConfig->msgRam.pTxDedBuf + (buffer *
            (mcanConfig->txBufElmtSize & ELMT_SIZE_MASK));
        if ( idType == MCAN_STD_ID )
            *pThisTxBuf++ = (( id << 18 ) & ( CAN_11_BIT_ID_MASK << 18 ));
       else
            *pThisTxBuf++ = BUFFER_XTD_MASK | ( id & CAN_29_BIT_ID_MASK );
        *pThisTxBuf++ = (uint32_t) dlc << 16;
        /* enable transmit from buffer to set TC interrupt bit in IR, but
        interrupt will not happen unless TC interrupt is enabled*/
        mcan->MCAN_TXBTIE = ( 1 << buffer) ;
    }
    SCB_CleanInvalidateDCache();
    return (uint8_t *) pThisTxBuf;  // now it points to the data field
}

/**
 * \brief Send Tx buffer.
 * \param mcanConfig  Pointer to a MCAN instance.
 * \param buffer  Pointer to buffer.
 */
void MCAN_SendTxDedBuffer( const MCan_ConfigType * mcanConfig, uint8_t buffer )
{
    Mcan * mcan = mcanConfig->pMCan;

    if ( buffer < mcanConfig->nmbrTxDedBufElmts ) {
      mcan->MCAN_TXBAR = ( 1 << buffer );
    }
}

/**
 * \brief Adds Message to TX Fifo / Queue
 * \param mcanConfig  Pointer to a MCAN instance.
 * \param id  Message ID.
 * \param idType  Type of ID
 * \param dlc  Type of dlc.
 * \param data  Pointer to data.
 */
uint32_t MCAN_AddToTxFifoQ( const MCan_ConfigType * mcanConfig,
        uint32_t id, MCan_IdType idType, MCan_DlcType dlc, uint8_t * data )
{
    Mcan * mcan = mcanConfig->pMCan;
    uint32_t   putIdx = 255;
    uint32_t * pThisTxBuf = 0;
    uint8_t  * pTxData;
    uint8_t    cnt;

    // Configured for FifoQ and FifoQ not full?
    if (( mcanConfig->nmbrTxFifoQElmts > 0 ) &&
            (( mcan->MCAN_TXFQS & MCAN_TXFQS_TFQF ) == 0 )) {
        putIdx = ( mcan->MCAN_TXFQS & MCAN_TXFQS_TFQPI_Msk ) >> MCAN_TXFQS_TFQPI_Pos;
        pThisTxBuf = mcanConfig->msgRam.pTxDedBuf + (putIdx *
            (mcanConfig->txBufElmtSize & ELMT_SIZE_MASK));
        if ( idType == MCAN_STD_ID )
            *pThisTxBuf++ = (( id << 18 ) & ( CAN_11_BIT_ID_MASK << 18 ));
        else
            *pThisTxBuf++ = BUFFER_XTD_MASK | ( id & CAN_29_BIT_ID_MASK );
        *pThisTxBuf++ = (uint32_t) dlc << 16;
        pTxData = (uint8_t *) pThisTxBuf;
        for ( cnt = 0; cnt < dlc ; cnt++ ) {
            *pTxData++ = *data++;
        }
        /* enable transmit from buffer to set TC interrupt bit in IR, but
        interrupt will not happen unless TC interrupt is enabled */
        mcan->MCAN_TXBTIE = ( 1 << putIdx);
        // request to send
        mcan->MCAN_TXBAR = ( 1 << putIdx );
    }
    SCB_CleanInvalidateDCache();
    return putIdx;  // now it points to the data field
}

/**
 * \brief Check if data transmitted from buffer/fifo/queue
 * \param mcanConfig  Pointer to a MCAN instance.
 * \param buffer  Pointer to data buffer.
 */
uint8_t MCAN_IsBufferTxd( const MCan_ConfigType * mcanConfig, uint8_t buffer )
{
    Mcan * mcan = mcanConfig->pMCan;

    return ( mcan->MCAN_TXBTO & ( 1 << buffer ) );
}

/**
 * \brief Configure RX Buffer Filter
 * ID must match exactly for a RX Buffer Filter
 * \param mcanConfig  Pointer to a MCAN instance.
 * \param buffer  Pointer to data buffer.
 * \param filter  data of filter.
 * \param idType  Type of ID
 */
void MCAN_ConfigRxBufferFilter( const MCan_ConfigType * mcanConfig,
        uint32_t buffer, uint32_t filter, uint32_t id, MCan_IdType idType)
{
    uint32_t * pThisRxFilt = 0;

    if ( buffer < mcanConfig->nmbrRxDedBufElmts ) {
        if ( idType == MCAN_STD_ID ) {
            if (( filter < mcanConfig->nmbrStdFilts )
                    && ( id <= CAN_11_BIT_ID_MASK )) {
                pThisRxFilt = mcanConfig->msgRam.pStdFilts + filter;
                // 1 word per filter
                *pThisRxFilt = STD_FILT_SFEC_BUFFER | (id << 16) |
                        STD_FILT_SFID2_RX_BUFFER | buffer;
            }
        } else {
        // extended ID
            if (( filter < mcanConfig->nmbrExtFilts ) &&
                    ( id <= CAN_29_BIT_ID_MASK )) {
                pThisRxFilt = mcanConfig->msgRam.pExtFilts + (2 * filter);
                // 2 words per filter
                *pThisRxFilt++ = (uint32_t) EXT_FILT_EFEC_BUFFER | id;
                *pThisRxFilt = EXT_FILT_EFID2_RX_BUFFER | buffer;
            }
        }
    }
    SCB_CleanInvalidateDCache();
}

/**
 * \brief Configure Classic Filter
 * Classic Filters direct accepted messages to a FIFO & include both a ID and
 * a ID mask
 * \param mcanConfig  Pointer to a MCAN instance.
 * \param buffer  Pointer to data buffer.
 * \param fifo   fifo Number.
 * \param filter  data of filter.
 * \param idType  Type of ID
 * \param mask  Mask to be match
 */
void MCAN_ConfigRxClassicFilter( const MCan_ConfigType * mcanConfig,
        MCan_FifoType fifo, uint8_t filter, uint32_t id,
        MCan_IdType idType, uint32_t mask )
{
    uint32_t * pThisRxFilt = 0;
    uint32_t   filterTemp;

    if ( idType == MCAN_STD_ID ) {
        if (( filter < mcanConfig->nmbrStdFilts ) && ( id <= CAN_11_BIT_ID_MASK )
                && ( mask <= CAN_11_BIT_ID_MASK )) {
            pThisRxFilt = mcanConfig->msgRam.pStdFilts + filter;
            // 1 word per filter
            filterTemp = (uint32_t) STD_FILT_SFT_CLASSIC | (id << 16) | mask;
            if ( fifo == MCAN_FIFO_0 ) {
                *pThisRxFilt = STD_FILT_SFEC_FIFO0 | filterTemp;
            } else if ( fifo == MCAN_FIFO_1 ) {
                *pThisRxFilt = STD_FILT_SFEC_FIFO1 | filterTemp;
            }
        } else {
            // extended ID
            if (( filter < mcanConfig->nmbrExtFilts )
                    && ( id <= CAN_29_BIT_ID_MASK )
                    && ( mask <= CAN_29_BIT_ID_MASK )) {
                pThisRxFilt = mcanConfig->msgRam.pExtFilts + (2 * filter);
                // 2 words per filter
                if ( fifo == MCAN_FIFO_0 ) {
                    *pThisRxFilt++ = EXT_FILT_EFEC_FIFO0 | id;
                } else if ( fifo == MCAN_FIFO_1 ) {
                    *pThisRxFilt++ = EXT_FILT_EFEC_FIFO1 | id;
                }
                *pThisRxFilt = (uint32_t) EXT_FILT_EFT_CLASSIC | mask;
            }
        }
    }
    SCB_CleanInvalidateDCache();
}

/**
 * \brief check if data received into buffer
 * \param mcanConfig  Pointer to a MCAN instance.
 * \param buffer  Pointer to data buffer.
 */
uint8_t MCAN_IsNewDataInRxDedBuffer( const MCan_ConfigType * mcanConfig,
                                uint8_t buffer )
{
    Mcan * mcan = mcanConfig->pMCan;

    SCB_CleanInvalidateDCache();

    if ( buffer < 32 ) {
      return ( mcan->MCAN_NDAT1 & ( 1 << buffer ));
    } else if ( buffer < 64 ) {
      return ( mcan->MCAN_NDAT1 & ( 1 << (buffer - 32 )));
    }
    else
      return 0;
}

/**
 * \brief Get Rx buffer
 * \param mcanConfig  Pointer to a MCAN instance.
 * \param buffer  Pointer to data buffer.
 * \param pRxMailbox  Pointer to rx Mailbox.
 */
void MCAN_GetRxDedBuffer( const MCan_ConfigType * mcanConfig,
                    uint8_t buffer, Mailbox64Type * pRxMailbox )
{
    Mcan     * mcan = mcanConfig->pMCan;
    uint32_t * pThisRxBuf = 0;
    uint32_t   tempRy;  // temp copy of RX buffer word
    uint8_t  * pRxData;
    uint8_t    idx;

    SCB_CleanInvalidateDCache();

    if ( buffer < mcanConfig->nmbrRxDedBufElmts ) {
        pThisRxBuf = mcanConfig->msgRam.pRxDedBuf
            + (buffer * (mcanConfig->rxBufElmtSize & ELMT_SIZE_MASK));
        tempRy = *pThisRxBuf++;  // word R0 contains ID
        if ( tempRy & BUFFER_XTD_MASK ) {
            // extended ID?
            pRxMailbox->info.id = tempRy & BUFFER_EXT_ID_MASK;
        } else {
            // standard ID
            pRxMailbox->info.id = ( tempRy & BUFFER_STD_ID_MASK) >> 18;
        }
        tempRy = *pThisRxBuf++;  // word R1 contains DLC & time stamp
        pRxMailbox->info.length = (tempRy & BUFFER_DLC_MASK) >> 16;
        pRxMailbox->info.timestamp = tempRy & BUFFER_RXTS_MASK;
        // copy the data from the buffer to the mailbox
        pRxData = (uint8_t *) pThisRxBuf;
        for ( idx = 0; idx < pRxMailbox->info.length; idx++ )
            pRxMailbox->data[idx] = *pRxData++;
        /* clear the new data flag for the buffer */
        if ( buffer < 32 ) {
            mcan->MCAN_NDAT1 = ( 1 << buffer );
        } else {
            mcan->MCAN_NDAT1 = ( 1 << (buffer - 32 ));
        }
    }
}

/**
 * \brief Get from the receive FIFO and place in a application mailbox
 * \param mcanConfig  Pointer to a MCAN instance.
 * \param fifo  Fifo Number
 * \param pRxMailbox  Pointer to rx Mailbox.
 * \return: # of fifo entries at the start of the function
 *         0 -> FIFO was empty at start
 *         1 -> FIFO had 1 entry at start, but is empty at finish
 *         2 -> FIFO had 2 entries at start, has 1 entry at finish
 */
uint32_t MCAN_GetRxFifoBuffer( const MCan_ConfigType * mcanConfig,
        MCan_FifoType fifo, Mailbox64Type * pRxMailbox )
{
    Mcan     * mcan = mcanConfig->pMCan;
    uint32_t * pThisRxBuf = 0;
    uint32_t   tempRy;  // temp copy of RX buffer word
    uint8_t  * pRxData;
    uint8_t    idx;
    uint32_t * fifo_ack_reg;
    uint32_t   get_index;
    uint32_t   fill_level;
    uint32_t   element_size;

    SCB_CleanInvalidateDCache();

    // default: fifo empty
    fill_level = 0;

    if ( fifo == MCAN_FIFO_0 ) {
        get_index = ( mcan->MCAN_RXF0S & MCAN_RXF0S_F0GI_Msk ) >> MCAN_RXF0S_F0GI_Pos;
        fill_level = ( mcan->MCAN_RXF0S & MCAN_RXF0S_F0FL_Msk ) >> MCAN_RXF0S_F0FL_Pos;
        pThisRxBuf = mcanConfig->msgRam.pRxFifo0;
        element_size = mcanConfig->rxFifo0ElmtSize & ELMT_SIZE_MASK;
        fifo_ack_reg = (uint32_t *) &mcan->MCAN_RXF0A;
    } else if ( fifo == MCAN_FIFO_1 ) {
        get_index = ( mcan->MCAN_RXF1S & MCAN_RXF1S_F1GI_Msk ) >> MCAN_RXF1S_F1GI_Pos;
        fill_level = ( mcan->MCAN_RXF1S & MCAN_RXF1S_F1FL_Msk ) >> MCAN_RXF1S_F1FL_Pos;
        pThisRxBuf = mcanConfig->msgRam.pRxFifo1;
        element_size = mcanConfig->rxFifo1ElmtSize & ELMT_SIZE_MASK;
        fifo_ack_reg = (uint32_t *) &mcan->MCAN_RXF1A;
    }

    if ( fill_level > 0 ) {
        pThisRxBuf = pThisRxBuf + (get_index * element_size);
        tempRy = *pThisRxBuf++;  // word R0 contains ID
        if ( tempRy & BUFFER_XTD_MASK ) {
            // extended ID?
            pRxMailbox->info.id = tempRy & BUFFER_EXT_ID_MASK;
        } else {
            // standard ID
            pRxMailbox->info.id = ( tempRy & BUFFER_STD_ID_MASK) >> 18;
        }
        tempRy = *pThisRxBuf++;  // word R1 contains DLC & timestamps
        pRxMailbox->info.length = (tempRy & BUFFER_DLC_MASK) >> 16;
        pRxMailbox->info.timestamp = tempRy & BUFFER_RXTS_MASK;
        /* copy the data from the buffer to the mailbox */
        pRxData = (uint8_t *) pThisRxBuf;
        for ( idx = 0; idx < pRxMailbox->info.length; idx++ )
            pRxMailbox->data[idx] = *pRxData++;
        // acknowledge reading the fifo entry
        *fifo_ack_reg = get_index;
        /* return entries remaining in FIFO */
    }
    return ( fill_level );
}


//----��4��u8�����ϲ�Ϊһ��u32����-------------------------------------
//����: ��CAN֡��8���ֽڲ��Ϊ2��32λ��U32�������˳��ΪDB3(��8λ)��DB2(�θ�8λ)��
//      DB1(�ε�8λ)��DB0(��8λ)��DB7~DB4�������ơ�
//����: //buf,Ҫ�ϳɵ��ĸ����������׵�ַ
//����: ����ʽ�ϳɺ��u32��
//-----------------------------------------------------------------------------
static bool_t MCAN_IsTxComplete(MCan_ConfigType * mcanConfig)
{
    Mcan * mcan = mcanConfig->pMCan;
    return ( mcan->MCAN_IR & MCAN_IR_TC );
}

//----��4��u8�����ϲ�Ϊһ��u32����-------------------------------------
//����: ��CAN֡��8���ֽڲ��Ϊ2��32λ��U32�������˳��ΪDB3(��8λ)��DB2(�θ�8λ)��
//      DB1(�ε�8λ)��DB0(��8λ)��DB7~DB4�������ơ�
//����: //buf,Ҫ�ϳɵ��ĸ����������׵�ַ
//����: ����ʽ�ϳɺ��u32��
//-----------------------------------------------------------------------------
static void MCAN_ClearTxComplete( MCan_ConfigType * mcanConfig )
{
    Mcan * mcan = mcanConfig->pMCan;
    mcan->MCAN_IR = MCAN_IR_TC;
}
//----��4��u8�����ϲ�Ϊһ��u32����-------------------------------------
//����: ��CAN֡��8���ֽڲ��Ϊ2��32λ��U32�������˳��ΪDB3(��8λ)��DB2(�θ�8λ)��
//      DB1(�ε�8λ)��DB0(��8λ)��DB7~DB4�������ơ�
//����: //buf,Ҫ�ϳɵ��ĸ����������׵�ַ
//����: ����ʽ�ϳɺ��u32��
//-----------------------------------------------------------------------------
static uint32_t __ComByteToWords(uint8_t *buf)
{
   uint32_t val=0x00000000;
   uint8_t i;
   for(i=0;i<4;i++)
   {
       val|=((*(buf+i))<<(8*i));
   }
   return val;
}
//----CAN���ͺ���-------------------------------------
//����: CAN���ͺ���
//����:
       // byChip:  CAN������ѡ��   0~4(Ϊ������Ŀ����룬�����˲�������SR5333�иò���
//        Ϊ0��SR5333 V1.01�汾��ֻʹ����һ��CAN������)
       // txBuf:   ���ĵķ��ͻ�����
       // len:     ���ͱ��ĵĳ��ȣ�С�ڵ���8�ֽ�
//����: �ɹ����͵ĳ���
//-----------------------------------------------------------------------------
uint32_t CAN_WriteData(uint8_t byChip, uint8_t* txBuf, uint32_t len)
{
    uint32_t *pDataStartAddr;
    uint32_t Id,Len;
    uint8_t pkgnum,i,j;
    uint32_t tmp;

    //�������
    if(txBuf==NULL)
        return 0;
    if((len%13!=0)||(len==0))
    {
        gs_u64SndPkgBadCnt++;
        return 0;
    }

    pkgnum=len/13;
    gs_u64AppSndCnt+=pkgnum;
    if(pkgnum>=MCAN1_NMBR_TX_DED_BUF_ELMTS)
        return 0;
    for(i=0;i<pkgnum;i++)
    {
        Id=0x0000;
        Id|=(uint32_t)txBuf[1+13*i]<<24;
        Id|=(uint32_t)txBuf[2+13*i]<<16;
        Id|=(uint32_t)txBuf[3+13*i]<<8;
        Id|=(uint32_t)txBuf[4+13*i];
        pDataStartAddr=MCAN_ConfigTxDedBuffer( &mcan1Config,i,Id,MCAN_EXT_ID,\
               CAN_DLC_8);
        tmp=__ComByteToWords(&txBuf[5]);
        *pDataStartAddr=tmp;
        tmp=__ComByteToWords(&txBuf[9]);
        *(pDataStartAddr+1)=tmp;
        MCAN_SendTxDedBuffer(&mcan1Config,i);
        //�ȴ�10mS,��ȷ�Ϸ������
        DJY_EventDelay(10*mS);
        //��ѯ�Ƿ�ɹ����ͳ�ȥ
        if(MCAN_IsBufferTxd(&mcan1Config,i))
        {
            gs_u64HardSndCnt++;
            Len+=13;
        }
        //CAN����ʱץȡ���10�η��ͱ���
        if(gs_CanDebugFlag)
        {
            if(gs_SndDebugCnt<CN_DEBUG_CAN_CNT)
            {
                for(j=0;j<13;j++)
                {
                   gs_SndPkg[gs_SndDebugCnt*13+j]=txBuf[j];
                }
                gs_SndDebugCnt++;
            }
        }
    }

    return Len;
}


//----CAN���պ���-------------------------------------
//����: CAN���ͺ���
//����:
      // byChip:  CAN������ѡ��   0~4(Ϊ������Ŀ����룬�����˲�������SR5333�иò���
      //  Ϊ0��SR5333 V1.01�汾��ֻʹ����һ��CAN������)
      // txBuf:    ���ĵĽ��ջ�����
      // len:     �����ȡ�ĳ���
      // pRd:     ��������ָ��
//����: �ɹ���ȡ���ݵĳ���
//-----------------------------------------------------------------------------
uint32_t CAN_ReadData(uint8_t byChip, uint8_t* rxBuf, uint32_t len, uint32_t *pRd)
{
    uint32_t rdLen=0x0000,ringlen,pkgnum;
     //�����������
     if(rxBuf==NULL)
         return 0;
     ringlen=Ring_Check(gs_RxRingBuf);
     if(len>ringlen)
         return 0;
     rdLen=Ring_Read(gs_RxRingBuf,rxBuf,len);
     pkgnum=rdLen/13;
     gs_u64AppRcvCnt+=pkgnum;
     return rdLen;
}

//----�����ж���Ӧ����---------------------------------------------------------
//����: ���β�ѯר�ý���Buffer(Dedicated RX Buffer)�������������
//����: pCanPara,CAN���������ò����ṹ��ָ��;
//      bufid,ר��Tx buffer���
//����: ��
//-----------------------------------------------------------------------------
static void __WriteMsgToRing(Mailbox64Type * rxMailbox)
{
    uint8_t i,len,j;
    uint8_t buf[13];
    uint32_t can_id,tmp;
    len=gs_rxMailbox.info.length;
    if(len!=8)
    {
        gs_u64RcvPkgBadCnt++;
    }
    else
    {
        buf[0]=0x88;
        can_id=gs_rxMailbox.info.id;
        tmp=can_id;
        buf[1]=tmp;
        buf[2]=tmp>>8;
        buf[3]=tmp>>16;
        buf[4]=tmp>>24;
        for(i=0;i<8;i++)
        {
            buf[5+i]=gs_rxMailbox.data[i];
        }
        Ring_Write(gs_RxRingBuf,buf,13);
        gs_u64HardRcvCnt++;
//CAN����ץȡ���10�ν��ձ���
        if(gs_CanDebugFlag)
        {
            if(gs_RcvDebugCnt<CN_DEBUG_CAN_CNT)
            {
                for(j=0;j<13;j++)
                {
                   gs_RcvPkg[gs_RcvDebugCnt*13+j]=buf[j];
                }
                gs_RcvDebugCnt++;
            }
        }
    }
}


//----�����ж���Ӧ����---------------------------------------------------------
//����: ���β�ѯר�ý���Buffer(Dedicated RX Buffer)�������������
//����: pCanPara,CAN���������ò����ṹ��ָ��;
//      bufid,ר��Tx buffer���
//����: ��
//-----------------------------------------------------------------------------
static void CAN_Msg_Hard_Read(vu32 byIR)
{
    uint8_t i;
    //���ݴ洢��Dedicated RX Buffer��
    if(byIR&MCAN_IR_DRX)
    {
       //�������жϱ�־λ
       if ( MCAN_IsMessageStoredToRxDedBuffer( &mcan1Config ) )
       {
           MCAN_ClearMessageStoredToRxBuffer( &mcan1Config );
            //���β�ѯר��Buffer���Ƿ��������ݣ�����
           for(i=0;i<MCAN1_NMBR_RX_DED_BUF_ELMTS;i++)
           {
              if ( MCAN_IsNewDataInRxDedBuffer( &mcan1Config, i ) )
              {
                  MCAN_GetRxDedBuffer( &mcan1Config, i, (Mailbox64Type *)\
                                                &gs_rxMailbox );
                  __WriteMsgToRing((Mailbox64Type *)&gs_rxMailbox );
              }
           }
       }
    }
    //���ݴ洢��FIFO0��
    else if(byIR&MCAN_IR_RF0N)
    {
        //�������ж�
        MCAN1->MCAN_IR = MCAN_IR_RF0N;
        MCAN_GetRxFifoBuffer(&mcan1Config,MCAN_FIFO_0, (Mailbox64Type *)\
                &gs_rxMailbox);
        __WriteMsgToRing((Mailbox64Type *)&gs_rxMailbox );
    }
    else
    {

    }
}

//----CAN�ж���Ӧ����------------------------------------------------------------
//����: CAN�ж���Ӧ����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void MCAN1_Line1_ISR_Handler(void)
{
    vu32 byIR;
    byIR=MCAN1->MCAN_IR;
    //���β�ѯ����ʹ���жϵ��ж�״̬λ������λ��λ����
    //�����ж�
    if(byIR&MCAN_IR_RF0N)
    {
        CAN_Msg_Hard_Read(byIR);
    }

    if(byIR&MCAN_IR_DRX)
    {
        CAN_Msg_Hard_Read(byIR);
    }
    //���߹ر�
    if(byIR&MCAN_IR_BO)
    {
        gs_u64BusOffCnt++;
        debug_printf("CAN","Bus Off. \r\n");
        MCAN_Init(&mcan1Config);
    }
    if(byIR&MCAN_IR_CRCE)
    {
        gs_u64CrcErrCnt++;
    }
    if(byIR&MCAN_IR_BE)
    {
        gs_u64BitErrCnt++;
    }
    if(byIR&MCAN_IR_FOE)
    {
        gs_u64FoErrCnt++;
    }
    if(byIR&MCAN_IR_STE)
    {
        gs_u64StErrCnt++;
    }
    if(byIR&MCAN_IR_ACKE)
    {
        gs_u64ACKErrCnt++;
    }
    if(byIR&MCAN_IR_MRAF)
    {
        //todo
    }
    if(byIR&MCAN_IR_WDI)
    {
        //todo
    }
}


//----���CANͨ�������շ����---------------------------------------------------------
//����: ���CANͨ�������շ��������2s��CAN�������ջ��߷�������û�б仯ʱ��������ΪCAN
//���������ֹ��ϣ���ʱ��Ҫ��CAN���������и�λ����.
//����: ��.
//����: ��.
//-----------------------------------------------------------------------------
ptu32_t __Can_Monitor(void)
{
   u64 sndcnt=0,rcvcnt=0;
   bool_t bQiYongFlag=false,bRstFlag=false;
   while(1)
   {
       bRstFlag=false;
       sndcnt=gs_u64HardSndCnt;
       rcvcnt=gs_u64HardRcvCnt;
       if((sndcnt!=0)||(rcvcnt!=0))
       {
           bQiYongFlag=true;
       }
       DJY_EventDelay(CN_CAN_DEV_MONITOR_TIME);
       if(bQiYongFlag)
       {
           if(sndcnt==gs_u64HardSndCnt)
           {
               gs_TxErrRstCnt++;
               bRstFlag=true;
               MCAN_Init(&mcan1Config);
           }

           if(!bRstFlag)
           {
              if(rcvcnt==gs_u64HardRcvCnt)
              {
                  gs_RxErrRstCnt++;
                  MCAN_Init(&mcan1Config);
              }
           }
       }

   }
   return 1;
}

//----��ʼ��CAN���ò���---------------------------------------------------------
//����: ��ʼ��CAN���ò���
//����: pCanPara,CAN���������ò����ṹ��ָ��
//����: �ɹ���ʼ���򷵻�true,�����򷵻�false.
//-----------------------------------------------------------------------------
bool_t ModuleInstall_CAN(void)
{
    uint16_t evtt;
    //�������ջ��λ�����
    gs_RxRingBuf=(struct RingBuf *)malloc(sizeof(struct RingBuf));
    if(gs_RxRingBuf==NULL)
        return false;
    gs_RxRingBuf->buf=malloc(CN_CAN_RX_BUF_SIZE);
    if(gs_RxRingBuf->buf==NULL)
    {
        free(gs_RxRingBuf);
        return false;
    }
    Ring_Init(gs_RxRingBuf,gs_RxRingBuf->buf,CN_CAN_RX_BUF_SIZE);
    //CAN�Ĵ�����ʼ��(�������ò����ʵ�)
    MCAN_Init(&mcan1Config);
    MCAN_Shell_Module_Install();
    evtt = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,__Can_Monitor,
              CAN_MonitorStack,sizeof(CAN_MonitorStack),"CAN Monitor function");
    if(evtt!=CN_EVTT_ID_INVALID)
    {
       DJY_EventPop(evtt,NULL,0,NULL,0,0);
    }
    else
    {
       DJY_EvttUnregist(evtt);
    }

    debug_printf("CAN","CAN install OK.\r\n");
    return true;

}
ADD_TO_ROUTINE_SHELL(canrst,canrst,"��λCAN������   COMMAND:canrst+enter");
ADD_TO_ROUTINE_SHELL(canstat,canstat,"CANͨ��ͳ��  COMMAND:canstat+enter");
ADD_TO_ROUTINE_SHELL(canreg,canreg,"��ȡCAN�Ĵ���ֵ   COMMAND:canreg+enter");
ADD_TO_ROUTINE_SHELL(canpkg,canpkg,"��ȡCAN���10�η���/���ձ�������    COMMAND:canpkg+enter");
