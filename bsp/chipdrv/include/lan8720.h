//----------------------------------------------------
// Copyright (c) 2018,Djyos Open source Development team. All rights reserved.

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
#ifndef __MLAN8720_H__
#define __MLAN8720_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define LAN8720_ADDR            0           //LAN8720��ַΪ0
#define LAN8720_TIMEOUT     ((uint32_t)500) //LAN8720��ʱʱ��

//LAN8720�Ĵ���
#define LAN8720_BCR      ((uint16_t)0x0000U)
#define LAN8720_BSR      ((uint16_t)0x0001U)
#define LAN8720_PHYI1R   ((uint16_t)0x0002U)
#define LAN8720_PHYI2R   ((uint16_t)0x0003U)
#define LAN8720_ANAR     ((uint16_t)0x0004U)
#define LAN8720_ANLPAR   ((uint16_t)0x0005U)
#define LAN8720_ANER     ((uint16_t)0x0006U)
#define LAN8720_ANNPTR   ((uint16_t)0x0007U)
#define LAN8720_ANNPRR   ((uint16_t)0x0008U)
#define LAN8720_MMDACR   ((uint16_t)0x000DU)
#define LAN8720_MMDAADR  ((uint16_t)0x000EU)
#define LAN8720_ENCTR    ((uint16_t)0x0010U)
#define LAN8720_MCSR     ((uint16_t)0x0011U)
#define LAN8720_SMR      ((uint16_t)0x0012U)
#define LAN8720_TPDCR    ((uint16_t)0x0018U)
#define LAN8720_TCSR     ((uint16_t)0x0019U)
#define LAN8720_SECR     ((uint16_t)0x001AU)
#define LAN8720_SCSIR    ((uint16_t)0x001BU)
#define LAN8720_CLR      ((uint16_t)0x001CU)
#define LAN8720_ISFR     ((uint16_t)0x001DU)
#define LAN8720_IMR      ((uint16_t)0x001EU)
#define LAN8720_PHYSCSR  ((uint16_t)0x001FU)


//LAN8720 BCR�Ĵ�����λ����
#define LAN8720_BCR_SOFT_RESET         ((uint16_t)0x8000U)
#define LAN8720_BCR_LOOPBACK           ((uint16_t)0x4000U)
#define LAN8720_BCR_SPEED_SELECT       ((uint16_t)0x2000U)
#define LAN8720_BCR_AUTONEGO_EN        ((uint16_t)0x1000U)
#define LAN8720_BCR_POWER_DOWN         ((uint16_t)0x0800U)
#define LAN8720_BCR_ISOLATE            ((uint16_t)0x0400U)
#define LAN8720_BCR_RESTART_AUTONEGO   ((uint16_t)0x0200U)
#define LAN8720_BCR_DUPLEX_MODE        ((uint16_t)0x0100U)

//LAN8720��BSR�Ĵ�����λ����
#define LAN8720_BSR_100BASE_T4       ((uint16_t)0x8000U)
#define LAN8720_BSR_100BASE_TX_FD    ((uint16_t)0x4000U)
#define LAN8720_BSR_100BASE_TX_HD    ((uint16_t)0x2000U)
#define LAN8720_BSR_10BASE_T_FD      ((uint16_t)0x1000U)
#define LAN8720_BSR_10BASE_T_HD      ((uint16_t)0x0800U)
#define LAN8720_BSR_100BASE_T2_FD    ((uint16_t)0x0400U)
#define LAN8720_BSR_100BASE_T2_HD    ((uint16_t)0x0200U)
#define LAN8720_BSR_EXTENDED_STATUS  ((uint16_t)0x0100U)
#define LAN8720_BSR_AUTONEGO_CPLT    ((uint16_t)0x0020U)
#define LAN8720_BSR_REMOTE_FAULT     ((uint16_t)0x0010U)
#define LAN8720_BSR_AUTONEGO_ABILITY ((uint16_t)0x0008U)
#define LAN8720_BSR_LINK_STATUS      ((uint16_t)0x0004U)
#define LAN8720_BSR_JABBER_DETECT    ((uint16_t)0x0002U)
#define LAN8720_BSR_EXTENDED_CAP     ((uint16_t)0x0001U)

//LAN8720 IMR/ISFR�Ĵ�����λ����
#define LAN8720_INT_7       ((uint16_t)0x0080U)
#define LAN8720_INT_6       ((uint16_t)0x0040U)
#define LAN8720_INT_5       ((uint16_t)0x0020U)
#define LAN8720_INT_4       ((uint16_t)0x0010U)
#define LAN8720_INT_3       ((uint16_t)0x0008U)
#define LAN8720_INT_2       ((uint16_t)0x0004U)
#define LAN8720_INT_1       ((uint16_t)0x0002U)

//LAN8720 PHYSCSR�Ĵ�����λ����
#define LAN8720_PHYSCSR_AUTONEGO_DONE   ((uint16_t)0x1000U)
#define LAN8720_PHYSCSR_HCDSPEEDMASK    ((uint16_t)0x001CU)
#define LAN8720_PHYSCSR_10BT_HD         ((uint16_t)0x0004U)
#define LAN8720_PHYSCSR_10BT_FD         ((uint16_t)0x0014U)
#define LAN8720_PHYSCSR_100BTX_HD       ((uint16_t)0x0008U)
#define LAN8720_PHYSCSR_100BTX_FD       ((uint16_t)0x0018U)

//LAN8720״̬��ض���
#define  LAN8720_STATUS_READ_ERROR            ((int32_t)-5)
#define  LAN8720_STATUS_WRITE_ERROR           ((int32_t)-4)
#define  LAN8720_STATUS_ADDRESS_ERROR         ((int32_t)-3)
#define  LAN8720_STATUS_RESET_TIMEOUT         ((int32_t)-2)
#define  LAN8720_STATUS_ERROR                 ((int32_t)-1)
#define  LAN8720_STATUS_OK                    ((int32_t) 0)
#define  LAN8720_STATUS_LINK_DOWN             ((int32_t) 1)
#define  LAN8720_STATUS_100MBITS_FULLDUPLEX   ((int32_t) 2)
#define  LAN8720_STATUS_100MBITS_HALFDUPLEX   ((int32_t) 3)
#define  LAN8720_STATUS_10MBITS_FULLDUPLEX    ((int32_t) 4)
#define  LAN8720_STATUS_10MBITS_HALFDUPLEX    ((int32_t) 5)
#define  LAN8720_STATUS_AUTONEGO_NOTDONE      ((int32_t) 6)

//LAN8720�жϱ�־λ����
#define  LAN8720_ENERGYON_IT                   LAN8720_INT_7
#define  LAN8720_AUTONEGO_COMPLETE_IT          LAN8720_INT_6
#define  LAN8720_REMOTE_FAULT_IT               LAN8720_INT_5
#define  LAN8720_LINK_DOWN_IT                  LAN8720_INT_4
#define  LAN8720_AUTONEGO_LP_ACK_IT            LAN8720_INT_3
#define  LAN8720_PARALLEL_DETECTION_FAULT_IT   LAN8720_INT_2
#define  LAN8720_AUTONEGO_PAGE_RECEIVED_IT     LAN8720_INT_1

typedef int32_t  (*lan8720_Init_Func) (void);
typedef int32_t  (*lan8720_DeInit_Func) (void);
typedef int32_t  (*lan8720_ReadReg_Func)   (uint32_t, uint32_t, uint32_t *);
typedef int32_t  (*lan8720_WriteReg_Func)  (uint32_t, uint32_t, uint32_t);

typedef struct
{
  lan8720_Init_Func      Init;
  lan8720_DeInit_Func    DeInit;
  lan8720_WriteReg_Func  WriteReg;
  lan8720_ReadReg_Func   ReadReg;
} lan8720_IOCtx_t;


typedef struct
{
  uint32_t            DevAddr;
  uint32_t            Is_Initialized;
  lan8720_IOCtx_t     IO;
  void               *pData;
}lan8720_Object_t;

int32_t LAN8720_ResetInit(void);
int32_t LAN8720_ReadPHY(u16 reg,u32 *regval);
int32_t LAN8720_WritePHY(u16 reg,u16 value);
void LAN8720_EnablePowerDownMode(void);
void LAN8720_DisablePowerDownMode(void);
void LAN8720_StartAutoNego(void);
void LAN8720_EnableLoopbackMode(void);
void LAN8720_DisableLoopbackMode(void);
void LAN8720_EnableIT(u32 interrupt);
void LAN8720_DisableIT(u32 interrupt);
void LAN8720_ClearIT(u32 interrupt);
u8 LAN8720_GetITStatus(u32 interrupt);
u32 LAN8720_GetLinkState(void);
void LAN8720_SetLinkState(u32 linkstate);

#ifdef __cplusplus
}
#endif
#endif
