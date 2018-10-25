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

// �ļ���     ��mcp2515.c
// ģ������: SPIתCANоƬmcp2515������
// ģ��汾: V1.00
// ������Ա: zhb
// ����ʱ��: 11/01.2017
// =============================================================================

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "os.h"
#include "djybus.h"
#include "spibus.h"
#include "mcp2515.h"
#include "cpu_peri.h"
#include "board-config.h"
#include "ring.h"
#include "shell.h"
#include "canbus.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//extern bool_t ModuleInstall_MCP2515(char *BusName,u32  baudrate);
//    if( ModuleInstall_MCP2515(CFG_MCP2515_BUS_NAME,CFG_MCP2515_SPEED) == false)
//    {
//        printf("MCP2515 Install Error!\r\n");
//        while( 1 );
//    }
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"mcp2515"              //��д�����������
//parent:"none"                         //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���                                           //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                                                       //ѡ���ѡ����ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ�IDE�ü������в���ʾ
//init time:medium                      //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                        //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
//dependence:"lock","spibus",           //������������������������none����ʾ�������������
                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//weakdependence:"none"                 //��������������������������none����ʾ�������������
                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//mutex:"none"                          //������������������������none����ʾ�������������
                                        //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //���ú�����ͷ�ļ����������б���
#ifndef CFG_MCP2515_SAM_MODE   //****�������Ƿ��Ѿ����ú�
#warning    MCP2515�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100,
#define CFG_MCP2515_SPEED                 (10*1000*1000)     //"SPI�����ٶ�",����MCP2515��SPI�����ٶ�
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_MCP2515_BUS_NAME              "SPI0"             //"SPI��������",MCP2515����������
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

/* Configuration Registers */
#define Mcp2515_CANSTAT         0x0E
#define Mcp2515_CANCTRL         0x0F
#define Mcp2515_BFPCTRL         0x0C
#define Mcp2515_TEC             0x1C
#define Mcp2515_REC             0x1D
#define Mcp2515_CNF3            0x28
#define Mcp2515_CNF2            0x29
#define Mcp2515_CNF1            0x2A
#define Mcp2515_CANINTE         0x2B
#define Mcp2515_CANINTF         0x2C
#define Mcp2515_EFLG            0x2D
#define Mcp2515_TXRTSCTRL       0x0D
/*  Recieve Filters */
#define Mcp2515_RXF0SIDH        0x00
#define Mcp2515_RXF0SIDL        0x01
#define Mcp2515_RXF0EID8        0x02
#define Mcp2515_RXF0EID0        0x03
#define Mcp2515_RXF1SIDH        0x04
#define Mcp2515_RXF1SIDL        0x05
#define Mcp2515_RXF1EID8        0x06
#define Mcp2515_RXF1EID0        0x07
#define Mcp2515_RXF2SIDH        0x08
#define Mcp2515_RXF2SIDL        0x09
#define Mcp2515_RXF2EID8        0x0A
#define Mcp2515_RXF2EID0        0x0B
#define Mcp2515_RXF3SIDH        0x10
#define Mcp2515_RXF3SIDL        0x11
#define Mcp2515_RXF3EID8        0x12
#define Mcp2515_RXF3EID0        0x13
#define Mcp2515_RXF4SIDH        0x14
#define Mcp2515_RXF4SIDL        0x15
#define Mcp2515_RXF4EID8        0x16
#define Mcp2515_RXF4EID0        0x17
#define Mcp2515_RXF5SIDH        0x18
#define Mcp2515_RXF5SIDL        0x19
#define Mcp2515_RXF5EID8        0x1A
#define Mcp2515_RXF5EID0        0x1B
/* Receive Masks */
#define Mcp2515_RXM0SIDH        0x20
#define Mcp2515_RXM0SIDL        0x21
#define Mcp2515_RXM0EID8        0x22
#define Mcp2515_RXM0EID0        0x23
#define Mcp2515_RXM1SIDH        0x24
#define Mcp2515_RXM1SIDL        0x25
#define Mcp2515_RXM1EID8        0x26
#define Mcp2515_RXM1EID0        0x27
/* Tx Buffer 0 */
#define Mcp2515_TXB0CTRL        0x30
#define Mcp2515_TXB0SIDH        0x31
#define Mcp2515_TXB0SIDL        0x32
#define Mcp2515_TXB0EID8        0x33
#define Mcp2515_TXB0EID0        0x34
#define Mcp2515_TXB0DLC         0x35
#define Mcp2515_TXB0D0          0x36
#define Mcp2515_TXB0D1          0x37
#define Mcp2515_TXB0D2          0x38
#define Mcp2515_TXB0D3          0x39
#define Mcp2515_TXB0D4          0x3A
#define Mcp2515_TXB0D5          0x3B
#define Mcp2515_TXB0D6          0x3C
#define Mcp2515_TXB0D7          0x3D
/* Tx Buffer 1 */
#define Mcp2515_TXB1CTRL        0x40
#define Mcp2515_TXB1SIDH        0x41
#define Mcp2515_TXB1SIDL        0x42
#define Mcp2515_TXB1EID8        0x43
#define Mcp2515_TXB1EID0        0x44
#define Mcp2515_TXB1DLC         0x45
#define Mcp2515_TXB1D0          0x46
#define Mcp2515_TXB1D1          0x47
#define Mcp2515_TXB1D2          0x48
#define Mcp2515_TXB1D3          0x49
#define Mcp2515_TXB1D4          0x4A
#define Mcp2515_TXB1D5          0x4B
#define Mcp2515_TXB1D6          0x4C
#define Mcp2515_TXB1D7          0x4D
/* Tx Buffer 2 */
#define Mcp2515_TXB2CTRL        0x50
#define Mcp2515_TXB2SIDH        0x51
#define Mcp2515_TXB2SIDL        0x52
#define Mcp2515_TXB2EID8        0x53
#define Mcp2515_TXB2EID0        0x54
#define Mcp2515_TXB2DLC         0x55
#define Mcp2515_TXB2D0          0x56
#define Mcp2515_TXB2D1          0x57
#define Mcp2515_TXB2D2          0x58
#define Mcp2515_TXB2D3          0x59
#define Mcp2515_TXB2D4          0x5A
#define Mcp2515_TXB2D5          0x5B
#define Mcp2515_TXB2D6          0x5C
#define Mcp2515_TXB2D7          0x5D
/* Rx Buffer 0 */
#define Mcp2515_RXB0CTRL        0x60
#define Mcp2515_RXB0SIDH        0x61
#define Mcp2515_RXB0SIDL        0x62
#define Mcp2515_RXB0EID8        0x63
#define Mcp2515_RXB0EID0        0x64
#define Mcp2515_RXB0DLC         0x65
#define Mcp2515_RXB0D0          0x66
#define Mcp2515_RXB0D1          0x67
#define Mcp2515_RXB0D2          0x68
#define Mcp2515_RXB0D3          0x69
#define Mcp2515_RXB0D4          0x6A
#define Mcp2515_RXB0D5          0x6B
#define Mcp2515_RXB0D6          0x6C
#define Mcp2515_RXB0D7          0x6D
/* Rx Buffer 1 */
#define Mcp2515_RXB1CTRL        0x70
#define Mcp2515_RXB1SIDH        0x71
#define Mcp2515_RXB1SIDL        0x72
#define Mcp2515_RXB1EID8        0x73
#define Mcp2515_RXB1EID0        0x74
#define Mcp2515_RXB1DLC         0x75
#define Mcp2515_RXB1D0          0x76
#define Mcp2515_RXB1D1          0x77
#define Mcp2515_RXB1D2          0x78
#define Mcp2515_RXB1D3          0x79
#define Mcp2515_RXB1D4          0x7A
#define Mcp2515_RXB1D5          0x7B
#define Mcp2515_RXB1D6          0x7C
#define Mcp2515_RXB1D7          0x7D
/*******************************************************************
*               Bit register masks                                *
*******************************************************************/
/* TXBnCTRL */
#define Mcp2515_TXREQ           0x08
#define Mcp2515_TXP             0x03
/* RXBnCTRL */
#define Mcp2515_RXM             0x60
#define Mcp2515_BUKT            0x04
/* CANCTRL */
#define Mcp2515_REQOP           0xE0
#define Mcp2515_ABAT            0x10
#define Mcp2515_OSM             0x08
#define Mcp2515_CLKEN           0x04
#define Mcp2515_CLKPRE          0x03
/* CANSTAT */
#define Mcp2515_REQOP           0xE0
#define Mcp2515_ICOD            0x0E
/* CANINTE */
#define Mcp2515_RX0IE           0x01
#define Mcp2515_RX1IE           0x02
#define Mcp2515_TX0IE           0x04
#define Mcp2515_TX1IE           0x80
#define Mcp2515_TX2IE           0x10
#define Mcp2515_ERRIE           0x20
#define Mcp2515_WAKIE           0x40
#define Mcp2515_MERRE           0x80
/* CANINTF */
#define Mcp2515_RX0IF           0x01
#define Mcp2515_RX1IF           0x02
#define Mcp2515_TX0IF           0x04
#define Mcp2515_TX1IF           0x80
#define Mcp2515_TX2IF           0x10
#define Mcp2515_ERRIF           0x20
#define Mcp2515_WAKIF           0x40
#define Mcp2515_MERRF           0x80
/* BFPCTRL */
#define Mcp2515_B1BFS           0x20
#define Mcp2515_B0BFS           0x10
#define Mcp2515_B1BFE           0x08
#define Mcp2515_B0BFE           0x04
#define Mcp2515_B1BFM           0x02
#define Mcp2515_B0BFM           0x01
/* CNF1 Masks */
#define Mcp2515_SJW             0xC0
#define Mcp2515_BRP             0x3F
/* CNF2 Masks */
#define Mcp2515_BTLMODE         0x80
#define Mcp2515_SAM             0x40
#define Mcp2515_PHSEG1          0x38
#define Mcp2515_PRSEG           0x07
/* CNF3 Masks */
#define Mcp2515_WAKFIL          0x40
#define Mcp2515_PHSEG2          0x07
/* TXRTSCTRL Masks */
#define Mcp2515_TXB2RTS         0x04
#define Mcp2515_TXB1RTS         0x02
#define Mcp2515_TXB0RTS         0x01
/*******************************************************************
*                    Bit Timing Configuration                     *
*******************************************************************/
/* CNF1 */
#define Mcp2515_SJW_1TQ         0x40
#define Mcp2515_SJW_2TQ         0x80
#define Mcp2515_SJW_3TQ         0x90
#define Mcp2515_SJW_4TQ         0xC0
/* CNF2 */
#define Mcp2515_BTLMODE_CNF3    0x80
#define Mcp2515_BTLMODE_PH1_IPT 0x00
#define Mcp2515_SMPL_3X         0x40
#define Mcp2515_SMPL_1X         0x00
#define Mcp2515_PHSEG1_8TQ      0x38
#define Mcp2515_PHSEG1_7TQ      0x30
#define Mcp2515_PHSEG1_6TQ      0x28
#define Mcp2515_PHSEG1_5TQ      0x20
#define Mcp2515_PHSEG1_4TQ      0x18
#define Mcp2515_PHSEG1_3TQ      0x10
#define Mcp2515_PHSEG1_2TQ      0x08
#define Mcp2515_PHSEG1_1TQ      0x00
#define Mcp2515_PRSEG_8TQ       0x07
#define Mcp2515_PRSEG_7TQ       0x06
#define Mcp2515_PRSEG_6TQ       0x05
#define Mcp2515_PRSEG_5TQ       0x04
#define Mcp2515_PRSEG_4TQ       0x03
#define Mcp2515_PRSEG_3TQ       0x02
#define Mcp2515_PRSEG_2TQ       0x01
#define Mcp2515_PRSEG_1TQ       0x00
/* CNF3 */
#define Mcp2515_PHSEG2_8TQ      0x07
#define Mcp2515_PHSEG2_7TQ      0x06
#define Mcp2515_PHSEG2_6TQ      0x05
#define Mcp2515_PHSEG2_5TQ      0x04
#define Mcp2515_PHSEG2_4TQ      0x03
#define Mcp2515_PHSEG2_3TQ      0x02
#define Mcp2515_PHSEG2_2TQ      0x01
#define Mcp2515_PHSEG2_1TQ      0x00
#define Mcp2515_SOF_ENABLED     0x80
#define Mcp2515_WAKFIL_ENABLED  0x40
#define Mcp2515_WAKFIL_DISABLED 0x00
/*******************************************************************
*                  Control/Configuration Registers                *
*******************************************************************/
/* CANINTE */
#define Mcp2515_RX0IE_ENABLED   0x01
#define Mcp2515_RX0IE_DISABLED  0x00
#define Mcp2515_RX1IE_ENABLED   0x02
#define Mcp2515_RX1IE_DISABLED  0x00
#define Mcp2515_G_RXIE_ENABLED  0x03
#define Mcp2515_G_RXIE_DISABLED 0x00
#define Mcp2515_TX0IE_ENABLED   0x04
#define Mcp2515_TX0IE_DISABLED  0x00
#define Mcp2515_TX1IE_ENABLED   0x08
#define Mcp2515_TX2IE_DISABLED  0x00
#define Mcp2515_TX2IE_ENABLED   0x10
#define Mcp2515_TX2IE_DISABLED  0x00
#define Mcp2515_G_TXIE_ENABLED  0x1C
#define Mcp2515_G_TXIE_DISABLED 0x00
#define Mcp2515_ERRIE_ENABLED   0x20
#define Mcp2515_ERRIE_DISABLED  0x00
#define Mcp2515_WAKIE_ENABLED   0x40
#define Mcp2515_WAKIE_DISABLED  0x00
#define Mcp2515_IVRE_ENABLED    0x80
#define Mcp2515_IVRE_DISABLED   0x00
/* CANINTF */
#define Mcp2515_RX0IF_SET       0x01
#define Mcp2515_RX0IF_RESET     0x00
#define Mcp2515_RX1IF_SET       0x02
#define Mcp2515_RX1IF_RESET     0x00
#define Mcp2515_TX0IF_SET       0x04
#define Mcp2515_TX0IF_RESET     0x00
#define Mcp2515_TX1IF_SET       0x08
#define Mcp2515_TX2IF_RESET     0x00
#define Mcp2515_TX2IF_SET       0x10
#define Mcp2515_TX2IF_RESET     0x00
#define Mcp2515_ERRIF_SET       0x20
#define Mcp2515_ERRIF_RESET     0x00
#define Mcp2515_WAKIF_SET       0x40
#define Mcp2515_WAKIF_RESET     0x00
#define Mcp2515_IVRF_SET        0x80
#define Mcp2515_IVRF_RESET      0x00
/* CANCTRL */
#define Mcp2515_REQOP_CONFIG    0x80
#define Mcp2515_REQOP_LISTEN    0x60
#define Mcp2515_REQOP_LOOPBACK  0x40
#define Mcp2515_REQOP_SLEEP     0x20
#define Mcp2515_REQOP_NORMAL    0x00
#define Mcp2515_ABORT           0x10
#define Mcp2515_OSM_ENABLED     0x08
#define Mcp2515_CLKOUT_ENABLED  0x04
#define Mcp2515_CLKOUT_DISABLED 0x00
#define Mcp2515_CLKOUT_PRE_8    0x03
#define Mcp2515_CLKOUT_PRE_4    0x02
#define Mcp2515_CLKOUT_PRE_2    0x01
#define Mcp2515_CLKOUT_PRE_1    0x00
/* CANSTAT */
#define Mcp2515_OPMODE_CONFIG   0x80
#define Mcp2515_OPMODE_LISTEN   0x60
#define Mcp2515_OPMODE_LOOPBACK 0x40
#define Mcp2515_OPMODE_SLEEP    0x20
#define Mcp2515_OPMODE_NORMAL   0x00
/* RXBnCTRL */
#define Mcp2515_RXM_RCV_ALL     0x60
#define Mcp2515_RXM_VALID_EXT   0x40
#define Mcp2515_RXM_VALID_STD   0x20
#define Mcp2515_RXM_VALID_ALL   0x00
#define Mcp2515_RXRTR_REMOTE    0x08
#define Mcp2515_RXRTR_NO_REMOTE 0x00
#define Mcp2515_BUKT_ROLLOVER    0x04
#define Mcp2515_BUKT_NO_ROLLOVER 0x00
#define Mcp2515_FILHIT0_FLTR_1  0x01
#define Mcp2515_FILHIT0_FLTR_0  0x00
#define Mcp2515_FILHIT1_FLTR_5  0x05
#define Mcp2515_FILHIT1_FLTR_4  0x04
#define Mcp2515_FILHIT1_FLTR_3  0x03
#define Mcp2515_FILHIT1_FLTR_2  0x02
#define Mcp2515_FILHIT1_FLTR_1  0x01
#define Mcp2515_FILHIT1_FLTR_0  0x00
/* TXBnCTRL */
#define Mcp2515_TXREQ_SET       0x08
#define Mcp2515_TXREQ_CLEAR     0x00
#define Mcp2515_TXP_HIGHEST     0x03
#define Mcp2515_TXP_INTER_HIGH  0x02
#define Mcp2515_TXP_INTER_LOW   0x01
#define Mcp2515_TXP_LOWEST      0x00
/*******************************************************************
*                  Register Bit Masks                             *
*******************************************************************/
#define Mcp2515_DLC_0          0x00
#define Mcp2515_DLC_1          0x01
#define Mcp2515_DLC_2          0x02
#define Mcp2515_DLC_3          0x03
#define Mcp2515_DLC_4          0x04
#define Mcp2515_DLC_5          0x05
#define Mcp2515_DLC_6          0x06
#define Mcp2515_DLC_7          0x07
#define Mcp2515_DLC_8          0x08
/*******************************************************************
*                  CAN SPI commands                               *
*******************************************************************/
#define Mcp2515_SPI_RESET       0xC0
#define Mcp2515_SPI_READ        0x03
#define Mcp2515_SPI_WRITE       0x02
#define Mcp2515_SPI_RTS         0x80
#define Mcp2515_SPI_RTS_TXB0    0x81
#define Mcp2515_SPI_RTS_TXB1    0x82
#define Mcp2515_SPI_RTS_TXB2    0x84
#define Mcp2515_SPI_READ_STATUS 0xA0
#define Mcp2515_SPI_BIT_MODIFY  0x05
#define Mcp2515_SPI_RX_STATUS   0xB0
#define Mcp2515_SPI_READ_RX          0x90
#define Mcp2515_SPI_WRITE_TX    0X40
/*******************************************************************
*                  Miscellaneous                                  *
*******************************************************************/
#define Mcp2515_DUMMY_BYTE      0x00
#define Mcp2515_TXB0            0x31
#define Mcp2515_TXB1            0x41
#define Mcp2515_TXB2            0x51
#define Mcp2515_RXB0            0x61
#define Mcp2515_RXB1            0x71
#define Mcp2515_EXIDE_SET       0x08
#define Mcp2515_EXIDE_RESET     0x00
/**************************************************************
*                  CAN Speed                                 *
*************************************************************/
#define Mcp2515_CAN_10Kbps          0x31
#define Mcp2515_CAN_25Kbps          0x13
#define Mcp2515_CAN_50Kbps             0x09
#define Mcp2515_CAN_100Kbps         0x04
#define Mcp2515_CAN_125Kbps            0x03
#define Mcp2515_CAN_250Kbps         0x01
#define Mcp2515_CAN_500Kbps         0x00
/*Frame Definition*/

#define MCP2515_SPI_SPEED      (1000*1000)    //1MHz
#define CN_MCP2515_MSGQ_LEN        13
#define CN_MCP2515_MSGQ_NUM       200
#define CN_MCP2515_RCV_LEN        (200*13)

static struct CANBusCB * p_MCP2515_CBHandle;//MCP2515��CAN���߿��ƿ�ṹ��,��ģ��ɼ�
static struct SPI_Device *s_ptMCP2515_Dev;
static u32 s_MCP2515_Timeout = 50*mS;

static bool_t sMCP2515Inited = false;

static struct SemaphoreLCB s_MCP2515_Semp;   //оƬ������ʱ���

static void MCP2515_CsActive(void);
static void MCP2515_CsInActive(void);
static void SPI_Send(uint8_t data);
bool_t MCP2515_Reset(void);

static u32 Mcp2515_baudrate=250;

typedef struct Frame_Strut
{
    unsigned int ID;
    unsigned char DLC;
    CanFrameType Type;
}Frame_TypeDef;

bool_t MCP2515_Read(uint8_t *CAN_RX_Buf,Frame_TypeDef *Frame);
bool_t MCP2515_Write(struct CANBusCB * CANBus,CanFarmeDef *Frame);
bool_t MCP2515_Ctrl(struct CANBusCB * CANBus,uint8_t ctrlcmd,ptu32_t param1,ptu32_t param2);

//=====================================================================
//��������Ƭѡ����
//������
//����ֵ��
//���ܣ�Ƭѡ���ͣ�ѡ������
//=====================================================================
static void MCP2515_CsActive(void)
{
     SPI_CsActive(s_ptMCP2515_Dev,CN_TIMEOUT_FOREVER);
}

//=====================================================================
//��������Ƭѡ����
//������
//����ֵ��
//���ܣ�Ƭѡ���ߣ���������
//=====================================================================
static void MCP2515_CsInActive(void)
{
    SPI_CsInactive(s_ptMCP2515_Dev);
}

//=====================================================================
//��������SPI���ͽ��պ���
//���������͵����ݵ�ַ�����ݳ��ȡ����ܵ����ݵ�ַ�����ݳ��ȡ�ƫ���������ͼ�����֮���ٿ�ʼ
//      ��һ����һ����ǰ���ǿն���
//����ֵ��
//���ܣ�SPI���ͽ���
//=====================================================================
static u32 MCP2515_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen,u32 RecvOff)
{
    struct SPI_DataFrame data;
    s32 result;

    data.RecvBuf = rdata;
    data.RecvLen = rlen;
    data.RecvOff = RecvOff;
    data.SendBuf = sdata;
    data.SendLen = slen;

    result = SPI_Transfer(s_ptMCP2515_Dev,&data,true,s_MCP2515_Timeout);
    if(result != CN_SPI_EXIT_NOERR)
        return 0;
    return 1;
}

//=====================================================================
//��������SPI����
//������
//����ֵ��
//���ܣ����������뷢�����ݣ�������
//=====================================================================
static void SPI_Send(uint8_t data)
{
     MCP2515_TxRx(&data,1,NULL,0,0);
}

/**
  * @brief  Send a byte to MCP2515 through SPI to return corresponding status you need
  * @param  Add:Address of register
  * @param  Data:the data U need to be sent from MCU to MCP2515
  * @retval none
  */
//=====================================================================
//��������д�Ĵ���
//�������Ĵ�����ַ����д�������
//����ֵ��
//���ܣ�ͨ��SPI��MCP2515����һ���ֽڽ����������״̬
//=====================================================================
void MCP2515_WriteReg(uint8_t Addr,uint8_t Data)
{
    MCP2515_CsActive();
    SPI_Send(Mcp2515_SPI_WRITE);
    SPI_Send(Addr);
    SPI_Send(Data);
    MCP2515_CsInActive();
}

/**
  * @brief  Send a command to MCP2515 through SPI to return corresponding status you need
  * @param  CMD:command
  * @retval Data:Data from MCP2515'SO pin
  */
//=====================================================================
//�����������Ĵ���ָ��
//����������ļĴ�����ַ
//����ֵ��
//���ܣ����͵�ƽ����˳���Ͷ�ָ�Ŀ���ַ���ٶ�һ������
//=====================================================================
uint8_t MCP2515_ReadByte(uint8_t Addr)
{
    u8 temp[3],i;
    u8 Data[3];
    temp[0]=Mcp2515_SPI_READ;
    temp[1]=Addr;
    temp[2]=0xaa;

    MCP2515_CsActive();

    for(i=0;i<3;i++)
    {
        MCP2515_TxRx(temp+i,1,Data+i,1,0);
    }

//    Djy_DelayUs(10);
    MCP2515_CsInActive();
    return Data[2];
}

/**
  * @brief  Reset the MCP2515 then U go into the configruation mode
  * @param  none
  * @retval none
  */
//=====================================================================
//����������λָ��
//������
//����ֵ��
//���ܣ���λָ��������³�ʼ��MCP2515 ���ڲ��Ĵ���������������ģʽ���ϵ��ʼ������һ����
//=====================================================================
bool_t MCP2515_Reset(void)
{
    u8 temp,Data;
    temp=Mcp2515_SPI_RESET;
    MCP2515_CsActive();
    Djy_DelayUs(100);
    MCP2515_TxRx(&temp,1,&Data,1,0);
    MCP2515_CsInActive();
    return true;
}

/**
  * @brief  Modify the bit of the register
  * @param  Add:Address of register
  * @param  Mask:1-->U can modify the corresponding bit;0-->U can not modify the corresponding bit
  * @param  Data:the data U need to be sent from MCU to MCP2515
  * @retval none
  */
//=====================================================================
//���������޸ļĴ�����λ
//�������Ĵ�����ַ�������ֽڡ������ֽ�
//����ֵ��
//���ܣ������ֽ���Ϊ1��λ�û��޸�Ϊ�����ֽ��ж�Ӧλ�õ�ֵ�������ֽ���Ϊ0��λ�ò���仯���ֲ���˵�ĺ���ϸ
//=====================================================================
void MCP2515_BitModify(uint8_t Addr,uint8_t Mask,uint8_t Data)
{
    MCP2515_CsActive();
    SPI_Send(Mcp2515_SPI_BIT_MODIFY);
    SPI_Send(Addr);
    SPI_Send(Mask);
    SPI_Send(Data);
    MCP2515_CsInActive();
}

/**
  * @brief  Send a command to MCP2515 through SPI to return corresponding status you need
  * @param  CMD:
                  SPI_RX_STATUS;
                SPI_READ_STATUS;
                SPI_READ_RX;
  * @retval Data:the status you need
  */
//=====================================================================
//������������ָ���
//������ָ��
//����ֵ��
//���ܣ���оƬ����һ��ָ���򵥵Ķ�д���ܣ���дһ�����ն�һ������дһ��0���ٶ�������ֵ
//=====================================================================
uint8_t SPI_CMD_MCP2515(uint8_t CMD)
{
    uint8_t Data;
    MCP2515_CsActive();
    MCP2515_TxRx(&CMD,1,&Data,1,1);
    MCP2515_CsInActive();
    return Data;
}

//----����CANͨ�Ų�����---------------------------------------------------------
//����: ����CANͨ�Ų����ʡ�
//����:baudrate,CANͨ�Ų����ʡ�
//����: �ɹ�����true,ʧ�ܷ���false.
//˵����
//-----------------------------------------------------------------------------
 bool_t MCP2515__SetBaudRate(u32 baudrate)
{
 /* ��������һ�²����ʵ����÷�����һ��������ʽ��
     TQ=2(BRP+1)/Fosc
         ������=1/(ͬ���γ�+�����γ�+PS1��+PS2��)*TQ
        ������⼴��    ����             BRP��CNF1�ֵ���ڣ��Ĵ���CNF1��BRP��ֵ+1��         Fosc��������Ƶ��8MHZ ����˿ɵ�TQ
        ������������֪��TQ��֪��������Ķγ���֮�ͣ�
        ͬ���γ�=1   �����γ�=���Ĵ���CNF2��PRSEG��ֵ+1��   PS1��=���Ĵ���CNF2��PHSEG1��ֵ+1��   PS2��=���Ĵ���CNF3��PHSEG2��ֵ+1��

        ʱ�����Ҫ����һ����ʽ����������60%-70%������������PS1ĩβ��
        ��    (ͬ���γ�+�����γ�+PS1��)/(ͬ���γ�+�����γ�+PS1��+PS2��)=60%-70%������������62.5%
        ����������
 */
     if(baudrate==250)
     {
       MCP2515_WriteReg(Mcp2515_CNF1,Mcp2515_CAN_250Kbps);

       MCP2515_WriteReg(Mcp2515_CNF2,Mcp2515_BTLMODE|Mcp2515_PHSEG1_3TQ|Mcp2515_PRSEG_1TQ);

       MCP2515_WriteReg(Mcp2515_CNF3,Mcp2515_PHSEG2_3TQ);
     }
     if(baudrate==125)
     {
       MCP2515_WriteReg(Mcp2515_CNF1,Mcp2515_CAN_125Kbps);
     //set CNF2,SAM=0,caputre one time,PHSEG1=(2+1)TQ=3TQ,PRSEG=(0+1)TQ=1TQ
     //����CNF2��SAM = 0��һ�ε��ף�PHSEG1 =��2 + 1��TQ = 3TQ��PRSEG =��0 + 1��TQ = 1TQ
       MCP2515_WriteReg(Mcp2515_CNF2,0x80|Mcp2515_PHSEG1_3TQ|Mcp2515_PRSEG_1TQ);
     //����CNF3��PHSEG2 =��2 + 1��TQ = 3TQ����CANCTRL.CLKEN = 1ʱ����CLKOUT��������Ϊ���
       MCP2515_WriteReg(Mcp2515_CNF3,Mcp2515_PHSEG2_3TQ);
     }
     return 1;

}

/**
  * @brief   Send n bytes with a given standard ID  corresponding to frame type
  * @param   CAN_TX_Buf: data to be sent
  * @param     DLC:DLC<=8
  * @param   SID:<=0x7FF
  * @param   CAN_FrameType:CAN_STD,CAN_RTR
  * @retval None
  */
//=====================================================================
//����������������
//��������ͨ��CAN���߷��ͳ�ȥ���������ڵĻ�����ָ�룬����һ���ṹ�壬���8�ֽ����ݣ�29λID�����ݳ��ȣ�֡����
//����ֵ��
//���ܣ���״̬���������д�������ֽڼĴ���������ݼĴ���һ���������־λ������
//     ���ͳ�ȥ��29λID˳�򲻶ԣ��������ݴ���ʱ���е���
//=====================================================================
void MCP2515_SendData(uint8_t *CAN_TX_Buf,Frame_TypeDef *Frame )
{
    uint8_t tempdata;
    uint8_t HSID,LSID,SIDLEID,HEID,LEID;
    u32 SID,EID;
    uint8_t tx_ctrl,tx_buf_addr,tx_cmd,reg_sidh,reg_sidl,reg_eidh,reg_eidl,reg_dlc;
    //Select an empty tx buffer//Ϊ0˵������
    if((MCP2515_ReadByte(Mcp2515_TXB0CTRL)&Mcp2515_TXREQ)==0)//TXBnCTRL��3��λ��TXREQ�����ķ�������λ
    {
        tx_ctrl=Mcp2515_TXB0CTRL;
        tx_buf_addr=Mcp2515_TXB0D0;//���ͻ���������д��ļĴ�����ַѡΪTXB0��
        tx_cmd=Mcp2515_SPI_RTS_TXB0;//���ͱ���ָ��ѡΪTXB0��
        reg_sidh=Mcp2515_TXB0SIDH;
        reg_sidl=Mcp2515_TXB0SIDL;
        reg_eidh=Mcp2515_TXB0EID8;
        reg_eidl=Mcp2515_TXB0EID0;
        reg_dlc=Mcp2515_TXB0DLC;
    }
    else if((MCP2515_ReadByte(Mcp2515_TXB1CTRL)&Mcp2515_TXREQ)==0)
    {
        tx_ctrl=Mcp2515_TXB1CTRL;
        tx_buf_addr=Mcp2515_TXB1D0;//���ͻ���������д��ļĴ�����ַѡΪTXB1��
        tx_cmd=Mcp2515_SPI_RTS_TXB1;//���ͱ���ָ��ѡΪTXB1��
        reg_sidh=Mcp2515_TXB1SIDH;
        reg_sidl=Mcp2515_TXB1SIDL;
        reg_eidh=Mcp2515_TXB1EID8;
        reg_eidl=Mcp2515_TXB1EID0;
        reg_dlc=Mcp2515_TXB1DLC;
    }
    else if((MCP2515_ReadByte(Mcp2515_TXB2CTRL)&Mcp2515_TXREQ)==0)
    {
        tx_ctrl=Mcp2515_TXB2CTRL;
        tx_buf_addr=Mcp2515_TXB2D0;//���ͻ���������д��ļĴ�����ַѡΪTXB2��
        tx_cmd=Mcp2515_SPI_RTS_TXB2;//���ͱ���ָ��ѡΪTXB2��
        reg_sidh=Mcp2515_TXB2SIDH;
        reg_sidl=Mcp2515_TXB2SIDL;
        reg_eidh=Mcp2515_TXB2EID8;
        reg_eidl=Mcp2515_TXB2EID0;
        reg_dlc=Mcp2515_TXB2DLC;
    }
    else
    {
        printf("No available tx buffer.\r\n");
//      Mcp2515_gs_u64SndPkgBadCnt++;
        return;
    }


    if(Frame->Type==EN_CAN_BUS_STD)//���ֲ�CAN����֡����STD�����׼����֡
    {
        /*Set the ID of the frame*//*����֡��ID */
        HSID=(uint8_t)(Frame->ID>>3);//��SID H��L �Ĵ���д��11λIDֵ���ֱ�д��10:3 2:0λ������Ҫ��һ��λ��
        LSID=(uint8_t)((Frame->ID<<5)&0xE0);//������λ��ʣ����λ����8λ��ĸ��ֽ�
        MCP2515_WriteReg(reg_sidh,HSID);
        MCP2515_WriteReg(reg_sidl,LSID);
        /*Set the DLC and the type of the frame*//*����DLC��֡������*/
        MCP2515_WriteReg(reg_dlc,Frame->DLC|EN_CAN_BUS_STD);//�������ݳ��ȣ��üĴ����ﻹ��һ��RTRλ
        /*Write the data into the TXB0 data registers */
        for(tempdata=0;tempdata<Frame->DLC;tempdata++)
        {
            MCP2515_WriteReg(tx_buf_addr+tempdata,CAN_TX_Buf[tempdata]);
        }
    }
    else if(Frame->Type==EN_CAN_BUS_RTR) /*if(CAN_FrameType==CAN_RTR)*///RTR
    {
        /*Set the ID of the frame*/
        HSID=(uint8_t)(Frame->ID>>3);
        LSID=(uint8_t)((Frame->ID<<5)&0xE0);
        MCP2515_WriteReg(Mcp2515_TXB0SIDH,HSID);
        MCP2515_WriteReg(Mcp2515_TXB0SIDL,LSID);
        /*Set the type of the frame*/
        MCP2515_WriteReg(Mcp2515_TXB0DLC,EN_CAN_BUS_RTR);
    }
    else if(Frame->Type==EN_CAN_BUS_EXD)//������չ֡
    {
        SID=Frame->ID>>18;//��չ֡�л���ID������29λ��ʶ���ĸ�11λ
        SID&=0x7ff;
        HSID=(uint8_t)(SID>>3);//������18λ��ֵ���Ǹ�11λ
        LSID=(uint8_t)((SID<<5)&0xE0);

        EID=Frame->ID &0x3FFFF;//ʣ��EID�е�18λ
        SIDLEID=(uint8_t)(EID>>16);//SIDL�е�2λ
        HEID=(uint8_t)(EID>>8);//����8λ֮��ǿ��ת��Ӧ�ÿ��Եõ�15:8λ
        LEID=(uint8_t)EID;//ֱ��ǿ��ת��ȥ����λ��ʣ��8λ���ǵ�8λ

        MCP2515_WriteReg(reg_sidh,HSID);
        MCP2515_WriteReg(reg_sidl,LSID | Mcp2515_EXIDE_SET | SIDLEID);//��չʹ���Լ�EID����λ

        MCP2515_WriteReg(reg_eidh,HEID);
        MCP2515_WriteReg(reg_eidl,LEID);

        MCP2515_WriteReg(reg_dlc,Frame->DLC);//�������ݳ��ȣ��üĴ����ﻹ��һ��RTRλ

        for(tempdata=0;tempdata<Frame->DLC;tempdata++)
        {
            MCP2515_WriteReg(tx_buf_addr+tempdata,CAN_TX_Buf[tempdata]);
        }
    }
    else
    {
        printf("No available type.\r\n");
        return;
    }

    /*Send the SPI_RTS_TXB0 request command to MCP2515 to send the data loaded in the data register*/
    MCP2515_CsActive();
    SPI_Send(tx_cmd);
    MCP2515_CsInActive();

    Djy_EventDelay(5*mS);
    tempdata=SPI_CMD_MCP2515(Mcp2515_SPI_READ_STATUS);
    if(0==(tempdata & 0x54))
    {
//      Mcp2515_gs_u64HardSndCnt++;
    }
    else
    {
//      Mcp2515_gs_u64SndPkgBadCnt++;
        //�ֶ��巢��״̬��־λ
        MCP2515_BitModify(tx_ctrl,Mcp2515_TXREQ,0x00);
    }
}

/**
  * @brief  Receive n bytes from MCP2515 RXB0
  * @param  none
  * @retval Data:return the effectively data from RXB0
  */
//=====================================================================
//����������������
//��������ŴӼĴ�����ȡ�������ݵĵ�ַ������֡
//����ֵ��
//���ܣ���״̬������������,����ֻ�������չ֡���ĵĽ���
//=====================================================================
void MCP2515_ReceiveData(void)
{
    uint8_t tempdata,i;
    uint8_t buf[13];
    u32 CAN_ID;
    u8 HSID,LSID,SIDLEID,HEID,LEID,DLC;
    u32 SID,EID;
    uint8_t CAN_RX_Buf[8];
    Frame_TypeDef Frame;

    tempdata=SPI_CMD_MCP2515(Mcp2515_SPI_RX_STATUS);//RX״ָ̬����յ��ı��ĵĻ����������͡��˲���
                                                    //��д��ָ���ٶ��ػ���дһ����һ����
    if(tempdata&0x40)//RXB0���յ����ģ���־λΪ1
    {
        HSID=MCP2515_ReadByte(Mcp2515_RXB0SIDH);//��29λ��ʶ����������
        LSID=MCP2515_ReadByte(Mcp2515_RXB0SIDL);//�����ĸ��Ĵ������
        HEID=MCP2515_ReadByte(Mcp2515_RXB0EID8);
        LEID=MCP2515_ReadByte(Mcp2515_RXB0EID0);

        if(LSID |Mcp2515_EXIDE_SET)//˵������չ֡������Ĵ������и���չ��ʶ����־λ
        {
            Frame.Type=EN_CAN_BUS_EXD;

            //�����ĸ��Ĵ���������ݴ���һ�£��Ƹ�λ���õ���ȷ��29λID
            SID=(LSID>>5);//SID2:0��������
            SID|=(HSID<<3);//SID10:3������λ��SID22:0�ϲ�
            SIDLEID=LSID & 0x03;
            EID=LEID;//EID��8λ
            EID|=(HEID<<8);//��EID��8λ�ϲ�
            EID|=(SIDLEID<<16);//��EID17:16λ�ϲ�

            CAN_ID=EID|(SID<<18);//�õ����ID

        }
        else//��׼֡���Ȳ���
        {
        }
        Frame.ID=CAN_ID;//�յ���ID���ṹ��
        DLC=MCP2515_ReadByte(Mcp2515_RXB0DLC);//��һ���м������ݣ�׼�������8�ֽ�����
        DLC=(DLC & 0x0F);
        if(DLC!=8)
        {
            DLC=MCP2515_ReadByte(Mcp2515_RXB0DLC);
            Djy_EventDelay(10*mS);
//          Mcp2515_gs_u64RcvPkgBadCnt++;
            MCP2515_BitModify(Mcp2515_CANINTF,Mcp2515_RX0IF_SET,0x00);
            return;
        }
        Frame.DLC=DLC;
        for(tempdata=0;tempdata<DLC;tempdata++)//�����ݱ�����������
        {
            CAN_RX_Buf[tempdata]=MCP2515_ReadByte(Mcp2515_RXB0D0+tempdata);
        }
        MCP2515_BitModify(Mcp2515_CANINTF,Mcp2515_RX0IF_SET,0x00);//һ���������еı��Ĵ�����ϣ����뽫��λ�����Խ�����һ������

        MCP2515_Read(CAN_RX_Buf,&Frame);
    }
//=============================================================================
    //һ��RXB0����ʤ�Σ������õ�
//=============================================================================
    if(tempdata&0x80)//RXB1���յ�����
    {
        HSID=MCP2515_ReadByte(Mcp2515_RXB1SIDH);
        LSID=MCP2515_ReadByte(Mcp2515_RXB1SIDL);
        HEID=MCP2515_ReadByte(Mcp2515_RXB1EID8);
        LEID=MCP2515_ReadByte(Mcp2515_RXB1EID0);

        if(LSID |Mcp2515_EXIDE_SET)//��չ֡
        {
            Frame.Type=EN_CAN_BUS_EXD;

            SID=LSID>>5;//SID2:0��������
            SID|=HSID<<3;//SID10:3������λ��SID22:0�ϲ�

            SIDLEID=LSID & 0x03;
            EID=LEID;//EID��8λ
            EID|=(HEID<<8);//��EID��8λ�ϲ�
            EID|=(SIDLEID<<16);//��EID17:16λ�ϲ�

            CAN_ID=EID|(SID<<18);//�õ����ID

        }
        else//��׼֡���Ȳ���
        {
        }


        Frame.ID=CAN_ID;//�յ���ID���ṹ��
        Djy_EventDelay(1*mS);
        DLC=MCP2515_ReadByte(Mcp2515_RXB1DLC);
        DLC=(DLC & 0x0F);
        if(DLC!=8)
        {
            Djy_EventDelay(10*mS);
//          Mcp2515_gs_u64RcvPkgBadCnt++;
            MCP2515_BitModify(Mcp2515_CANINTF,Mcp2515_RX0IF_SET,0x00);
            return;
        }
        Frame.DLC=DLC;
        for(tempdata=0;tempdata<DLC;tempdata++)
        {
            CAN_RX_Buf[tempdata]=MCP2515_ReadByte(Mcp2515_RXB1D0+tempdata);
        }
        MCP2515_BitModify(Mcp2515_CANINTF,Mcp2515_RX1IF,0x00);//һ���������еı��Ĵ�����ϣ����뽫��λ�����Խ�����һ������

        MCP2515_Read(CAN_RX_Buf,&Frame);
    }
}

//=====================================================================
//��������hal��GPIO���ⲿ�жϻص�����
//������
//����ֵ��
//���ܣ��жϵ���HAL_GPIO_EXTI_IRQHandler�����־λ��Ȼ����ñ��ص�����
//     ����������CAN2_Handleʵ��mcp2515���ձ��Ĳ���
//=====================================================================
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    u8 flag,eflg,tec,rec;
    u8 dummy=0;
    u32 DR=0;


    switch(GPIO_Pin)
    {
      case GPIO_PIN_3:
          Int_CutLine(MCP2515_EXIT_INT_LINE);           //�Ͽ��ж�

//        MCP2515_CsInActive();
          while((MCP2515_SPIX->SR & 0x01) != 0)//SPI���ݼĴ����Ƿ���δ��������
        {
            DR= *((__IO u8 *)&MCP2515_SPIX->DR) & 0xFFFF;//��ȡ�������Է������ȡ�����ݴ�λ
        }

          flag=MCP2515_ReadByte(Mcp2515_CANINTF); //�жϱ�־λ
          eflg=MCP2515_ReadByte(Mcp2515_EFLG);    //�����־λ
          tec=MCP2515_ReadByte(Mcp2515_TEC);      //���ʹ�������Ĵ���
          rec=MCP2515_ReadByte(Mcp2515_REC);      //���մ�������Ĵ���

          if(eflg==255 || flag==255)//��������ȡ�������⣬���¶�ȡ
          {
              tec=MCP2515_ReadByte(Mcp2515_TEC);
              rec=MCP2515_ReadByte(Mcp2515_REC);
              eflg=MCP2515_ReadByte(Mcp2515_EFLG);    //�����־λ
              flag=MCP2515_ReadByte(Mcp2515_CANINTF); //�жϱ�־λ
          }

          if(eflg & 0x80)                         //���ջ�����1���
          {
//            Mcp2515_gs_u64Rx1OverCnt++;
          }
          if(eflg & 0x40)                         //���ջ�����2���
          {
//            Mcp2515_gs_u64Rx0OverCnt++;
          }
          if(eflg & 0x20)                         //���߹رմ���
          {
//            Mcp2515_gs_u64BusOffCnt++;
//            Mcp2515_s_bBusOffFlag=true;
          }

          if(flag&(Mcp2515_RX1IF | Mcp2515_RX0IF))//���ջ�������
          {
//            Mcp2515_gs_u64IntCnt[0]++;
              MCP2515_ReceiveData();
          }

          if(flag & 0x80)
          {
//            Mcp2515_gs_u64IntCnt[1]++;
          }
          if(flag & 0x20)
          {
//            Mcp2515_gs_u64IntCnt[1]++;
          }

          if(eflg<=0x1f && eflg!=0)              //����������õĴ���
          {
//            Mcp2515_gs_u64IntCnt[1]++;

              MCP2515_WriteReg(Mcp2515_CANCTRL,Mcp2515_REQOP_CONFIG | Mcp2515_CLKOUT_ENABLED);//��������ģʽ
              dummy=MCP2515_ReadByte(Mcp2515_CANSTAT);
              MCP2515_WriteReg(Mcp2515_CANCTRL,Mcp2515_REQOP_NORMAL | Mcp2515_CLKOUT_ENABLED);//��������ģʽ
              dummy=MCP2515_ReadByte(Mcp2515_CANSTAT);
          }

          MCP2515_WriteReg(Mcp2515_EFLG,0);       //�����������б�־λ
          MCP2515_WriteReg(Mcp2515_CANINTF,0);

          Int_ClearLine(MCP2515_EXIT_INT_LINE);       //����ж��ߣ������ж�
          Int_ContactLine(MCP2515_EXIT_INT_LINE);
        break;
    }
}

//=====================================================================
//���������ϲ���÷��ͺ���
//������
//����ֵ��
//���ܣ�
//=====================================================================
bool_t MCP2515_Write(struct CANBusCB * CANBus,CanFarmeDef *Frame)
{
    uint8_t *CAN_TX_Buf;
    Frame_TypeDef MCP2515_Frame;

    MCP2515_Frame.ID = Frame->Id;
    MCP2515_Frame.DLC = Frame->DLC;
    MCP2515_Frame.Type = Frame->Type;
    CAN_TX_Buf = Frame->Data;

    MCP2515_SendData(CAN_TX_Buf,&MCP2515_Frame);
    return true;
}

//=====================================================================
//�������������ݷ����ϲ�ĺ���
//������
//����ֵ��
//���ܣ�
//=====================================================================
bool_t MCP2515_Read(uint8_t *CAN_RX_Buf,Frame_TypeDef *Frame)
{
    CanFarmeDef CAN_Frame;
    u8 i=0;

    CAN_Frame.Id=Frame->ID;
    CAN_Frame.DLC=Frame->DLC;
    CAN_Frame.Type=Frame->Type;

    for(i=0;i<CAN_Frame.DLC;i++)
    {
        CAN_Frame.Data[i]=CAN_RX_Buf[i];
    }
    CAN_BusAddRcvFrame(p_MCP2515_CBHandle,&CAN_Frame);

    return true;
}

//=====================================================================
//���������ϲ���ÿ��ƺ���
//������
//����ֵ��
//���ܣ�
//=====================================================================
bool_t MCP2515_Ctrl(struct CANBusCB * CANBus,uint8_t ctrlcmd,ptu32_t param1,ptu32_t param2)
{

    switch(ctrlcmd)
    {
       case EN_CAN_BUS_RESET:
               MCP2515_Reset();
               break;
       case EN_CAN_BUS_SET_BAUDRATE:
               switch(param1)
               {
                   case EN_CAN_BUS_BAUD_125K:
                       Mcp2515_baudrate=125;
                       break;
                   case EN_CAN_BUS_BAUD_250K:
                       Mcp2515_baudrate=250;
                       break;
                   default:
                       break;
               }
               if(Mcp2515_baudrate!=0)
                   MCP2515_Init();
                   break;
      case EN_CAN_BUS_SET_FILTER:

              break;
      default:
          break;
    }
    return true;
}

//=====================================================================
//���������ⲿ�жϳ�ʼ������
//������
//����ֵ��
//���ܣ�
//=====================================================================
void  MCP2515_IntInit(void)
{
    Int_Register(MCP2515_EXIT_INT_LINE);
    Int_IsrConnect(MCP2515_EXIT_INT_LINE,(void(*))HAL_GPIO_EXTI_IRQHandler);//�жϴ�������øú�����λ��hal_gpio.c��
    Int_SetIsrPara(MCP2515_EXIT_INT_LINE,GPIO_PIN_3);//������������3���ⲿ�ж���ͬ���źŵ�gpio�ڹ���һ���жϣ�
    Int_SettoAsynSignal(MCP2515_EXIT_INT_LINE);
    Int_ClearLine(MCP2515_EXIT_INT_LINE);
    Int_RestoreAsynLine(MCP2515_EXIT_INT_LINE);
}

//----MCP2515�ж�ʹ����ʧ��---------------------------------------------------------
//����: �򿪻����CANINTE�ж�ʹ�ܼĴ����ϵ�λ
//����: ��.
//����: ��.
//-----------------------------------------------------------------------------

void __MCP2515_CANINT_ENABLE(void)
{
    MCP2515_WriteReg(Mcp2515_CANINTE,Mcp2515_RX0IE|Mcp2515_RX1IE|Mcp2515_ERRIE|Mcp2515_MERRE);
}

void __MCP2515_CANINT_DISABLE(void)
{
    MCP2515_WriteReg(Mcp2515_CANINTE,0);
}

//=====================================================================
//����������ʼ��
//������
//����ֵ��
//���ܣ���λָ��������³�ʼ��MCP2515 ���ڲ��Ĵ���������������ģʽ���ϵ��ʼ������һ����
//=====================================================================
void MCP2515_Init(void)
{
    u8 flag;
    uint8_t dummy=0;
        /*���ȳ�ʼ��SPIȻ���ʼ��MCP2515
    ????1>ͨ����λMCP2515��������ģʽ��д��CANCTRL.REQOP��λ��
    ????2>���MCP2515�Ѿ���������ģʽ����ͨ��MCP2515_ReadByte��CANSTAT�����CANSTAT��
    ????3>���üĴ������ò����ʣ�CN1��CN2��CN3��
    ????4>����TXRTSCTRL������TXnRTS���ŵĹ��ܡ�
    ????5>����TRXBnCTRL���÷�������Ĺ��ܣ�TXB0��TXB1��TXB2��
    ????6>����TXBn'SID��EID��DLC������������������ģʽ�½������á�
    ????7>����RXFnSIDH��RXFnSIDL����ȡ�ض���ID��
    ????8>����RXMnSIDH��RXMnSIDL������RXFnSID��ID��
    ????9>����CANINTE�����û�����жϡ�
    ????10>���ص�����ģʽ���ٴμ��CANSTAT�Ƿ񷵻�����ģʽ��
    ????*/
    extern const Pin CAN_RstPin[];
    PIO_Set(&CAN_RstPin[0]);
    Djy_EventDelay(1*mS);
    PIO_Clear(&CAN_RstPin[0]);
    Djy_EventDelay(1*mS);
    PIO_Set(&CAN_RstPin[0]);
    Djy_EventDelay(1*mS);//Ӳ����λ

    /*��������ģʽ*/
    MCP2515_Reset();//�����λ
    Djy_EventDelay(10*mS);
    dummy=MCP2515_ReadByte(Mcp2515_CANSTAT);

    if(4!=(dummy>>5))
    {
        printf("δ��������״̬.\r\n");
        return;
    }
    Djy_EventDelay(1*mS);

    MCP2515__SetBaudRate(Mcp2515_baudrate);
    /*
    ???? 3 ���仺������TXB0��TXB1��TXB2
    ???? 2 ���ջ�������RXB0��RXB1
    ???? 6 ID��������RXF0?RXF5
    ???? 2 ID����Ĵ�����RXM0��RXM1
    */
    //����RXB0'�Ĵ���
    MCP2515_WriteReg(Mcp2515_RXB0CTRL,0x60);//��������CAN���ߵ�����֡
    MCP2515_WriteReg(Mcp2515_RXB1CTRL,0x60);//��������CAN���ߵ�����֡

    MCP2515_WriteReg(Mcp2515_RXM0SIDH,0x00);//Mask register//���μĴ���
    MCP2515_WriteReg(Mcp2515_RXM0SIDL,0x00);//��������λ���˲�
    MCP2515_WriteReg(Mcp2515_RXM1SIDH,0x00);//Mask register//���μĴ���
    MCP2515_WriteReg(Mcp2515_RXM1SIDL,0x00);//��������λ���˲�

     /*Set the RXB0 or RXB1 interrupt enableling ����RXB0��RXB1�ж�ʹ��*/
     //����RXB0��RXB1�ж�
     //���ô����жϣ����Ĵ����ж�
//     MCP2515_WriteReg(Mcp2515_CANINTE,Mcp2515_RX0IE|Mcp2515_RX1IE|Mcp2515_ERRIE|Mcp2515_MERRE);
    __MCP2515_CANINT_ENABLE();

     MCP2515_BitModify(Mcp2515_CANINTF,Mcp2515_RX0IF,0x00);//��־λ����
     MCP2515_BitModify(Mcp2515_CANINTF,Mcp2515_RX1IF,0x00);
     MCP2515_BitModify(Mcp2515_CANINTF,Mcp2515_ERRIF,0x00);//��־λ����
     MCP2515_BitModify(Mcp2515_CANINTF,Mcp2515_MERRF,0x00);

     MCP2515_WriteReg(Mcp2515_CANCTRL,Mcp2515_REQOP_NORMAL | Mcp2515_CLKOUT_ENABLED);//��������ģʽ
     dummy=MCP2515_ReadByte(Mcp2515_CANSTAT);
        if (Mcp2515_OPMODE_NORMAL != (dummy & 0xE0))
          MCP2515_WriteReg(Mcp2515_CANCTRL,Mcp2515_REQOP_NORMAL | Mcp2515_CLKOUT_ENABLED);//If normal ok

        if(0==dummy>>5)
        {
            printf("mode normal.\r\n");
        }
}

//----��MCP2515�豸�ҽӵ�SPIBUS��---------------------------------------------------------
//����:
//����: BusName����������
//
//����: �ɹ���ʼ���򷵻�true,ʧ���򷵻�false.
//-----------------------------------------------------------------------------
bool_t ModuleInstall_MCP2515(char *BusName,u32  baudrate)
{
    uint16_t evtt_id,event_id;
    struct CanDev Dev;
    struct CANBusCB * NewCANBus;

    if(NULL == Lock_SempCreate_s(&s_MCP2515_Semp,1,1,CN_BLOCK_FIFO,"MCP2515_semp"))
            return false;
        /* setup baud rate */

    if(NULL != SPI_DevAdd(BusName,"MCP2515",0,8,SPI_MODE_0,SPI_SHIFT_MSB,MCP2515_SPI_SPEED,false))
    {
        SPI_BusCtrl(s_ptMCP2515_Dev,CN_SPI_SET_POLL,0,0);
        sMCP2515Inited=true;
    }

    Mcp2515_baudrate=baudrate;

    Dev.CanCtrl=MCP2515_Ctrl;
    Dev.CanWrite=MCP2515_Write;
    Dev.ChipName="MCP2515";
    Dev.BaudRate=baudrate;
    Dev.PrivateTag=0;

    NewCANBus=CAN_BusAdd(&Dev);
    if(NewCANBus==NULL)
    {
        printf("CANBUS:MCAN1 Add failed.\r\n");
        return false;
    }
    p_MCP2515_CBHandle=NewCANBus;

    printf("CAN install OK.\r\n");
    return sMCP2515Inited;
}

