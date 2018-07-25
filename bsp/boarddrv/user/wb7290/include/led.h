// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��LED.H
// ģ������: led����ͷ�ļ�
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 07/03.2016
// =============================================================================

#ifndef __LED_H__
#define __LED_H__

#define LEDRUN     16

void Led_Init(void);
void Led_RunOn(void);
void Led_RunOff(void);
void Led_Out(u8 (*p)[4]);
#endif

