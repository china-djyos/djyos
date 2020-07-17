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

// �ļ���     ��board.c
// ģ������: �����ز��ֳ�ʼ�������õ�
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 5/11.2015
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "os.h"
#include "cpu_peri.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern void Board_Init(void);
//  Board_Init();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"board config"//�����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                      //ѡ�third��system��bsp��user����������������IDE�з���
//select:required                    //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                     //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                    //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                     //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"kernel","atsamv7","cpu onchip pio","cpu onchip gpio"//������������������������none����ʾ�������������
                                     //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                     //����������������������г����á�,���ָ�
//weakdependence:"none"              //��������������������������none����ʾ�������������
                                     //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                     //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                     //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_BOARD_CONFIG == false )
//#warning  " board_config  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header                //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_BOARD_CONFIG    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

//�������Ŷ���
static const Pin uart3_pin[] = {
    {PIO_PD28A_URXD3, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
    {PIO_PD30A_UTXD3, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT}};

static const Pin usart0_pin[] = {
    {PIO_PB0C_RXD0, PIOB, ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT},
    {PIO_PB1C_TXD0, PIOB, ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT}};

static const Pin usart1_pin[] = {
    {PIO_PA21A_RXD1, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT},
    {PIO_PB4D_TXD1, PIOB, ID_PIOB, PIO_PERIPH_D, PIO_DEFAULT}};

static const Pin Qspi_pin[] = {
        {(PIO_PA11A_QCS | PIO_PA13A_QIO0 | PIO_PA12A_QIO1 | PIO_PA17A_QIO2
                | PIO_PA14A_QSCK),PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT},
        {PIO_PD31A_QIO3, PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
};

static const Pin Gmac_pin[] = {
        {(PIO_PD0A_GTXCK | PIO_PD1A_GTXEN | PIO_PD2A_GTX0 | PIO_PD3A_GTX1
        | PIO_PD4A_GRXDV | PIO_PD5A_GRX0 | PIO_PD6A_GRX1 | PIO_PD7A_GRXER
        | PIO_PD8A_GMDC | PIO_PD9A_GMDIO ),PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},

        {PIO_PC30, PIOC, ID_PIOC, PIO_INPUT,    PIO_PULLUP},
        {PIO_PA29, PIOA, ID_PIOA, PIO_INPUT,    PIO_DEFAULT}
};

static const Pin Gmac_Reset_Pin[] = {
        {PIO_PC10, PIOC, ID_PIOC, PIO_OUTPUT_1, PIO_PULLUP}
};

static const Pin Phy_Interrupt_Pin[] = {
        {PIO_PA19, PIOA, ID_PIOA, PIO_INPUT,    PIO_IT_LOW_LEVEL}
};


void Board_UartHalfDuplexSend(u8 SerialNo)
{

}
void Board_UartHalfDuplexRecv(u8 SerialNo)
{

}


// =============================================================================
// ���ܣ���ʼ���������GPIO���ܣ�������ͨGPIO�ڣ���������IO(�紮�ڣ����ڵ�)
// ��������
// ���أ�true
// ˵�����ú����������������蹦������ǰ���ã�������module-trim��Sys_ModuleInit��һ��
//      ��������
// =============================================================================
bool_t Board_Init(void)
{
    PIO_Configure(usart0_pin, PIO_LISTSIZE(usart0_pin));
    PIO_Configure(usart1_pin, PIO_LISTSIZE(usart1_pin));
    PIO_Configure(uart3_pin, PIO_LISTSIZE(uart3_pin));

    PIO_Configure(Qspi_pin, PIO_LISTSIZE(Qspi_pin));

    //Ӳ��IO��λPHYоƬ
    PIO_Configure(Gmac_Reset_Pin, PIO_LISTSIZE(Gmac_Reset_Pin));
    PIO_Clear(Gmac_Reset_Pin);
    DJY_DelayUs(1000);
    PIO_Set(Gmac_Reset_Pin);

    //GMAC��IO���ų�ʼ��
    PIO_Configure(Gmac_pin, PIO_LISTSIZE(Gmac_pin));

    return true;
}
