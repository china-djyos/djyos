//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：mcp2515.c
// 模块描述: SPI转CAN芯片mcp2515的驱动
// 模块版本: V1.00
// 创建人员: zhb
// 创建时间: 11/01.2017
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

static struct SPI_Device s_MCP2515_Dev;
static u32 s_MCP2515_Timeout = 50*mS;

static bool_t sMCP2515Inited = false;

static struct SemaphoreLCB s_MCP2515_Semp;   //芯片互斥访问保护
static struct MsgQueue * gs_ptCanSndMsgQ=NULL;
static struct RingBuf * gs_MCP2525_RcvRing=NULL;

static void MCP2515_CsActive(void);
static void MCP2515_CsInActive(void);
static void SPI_Send(uint8_t data);
bool_t MCP2515_Reset(void);


static u64 gs_u64AppSndCnt;
static u64 gs_u64HardSndCnt;
static u64 gs_u64HardRcvCnt;
static u64 gs_u64AppRcvCnt;
static u64 gs_u64BusOffCnt;     //Bus off err
//static u64 gs_u64PassiveErrCnt; //Error passive flag
//static u64 gs_u64CrcErrCnt;     //Crc err
//static u64 gs_u64PasBitErrCnt;  //隐性位错误
//static u64 gs_u64DomBitErrCnt;  //显性位错误
//static u64 gs_u64FoErrCnt;      //format err
//static u64 gs_u64StErrCnt;      //bit stuff err
//static u64 gs_u64ACKErrCnt;     //ACK err
static u64 gs_MonitorErrRstCnt;
static u64 gs_u64RcvPkgBadCnt;
static u64 gs_u64SndPkgBadCnt;//
static u64 gs_u64RstFailedCnt;
static u64 gs_u64IntCnt;
static uint8_t MCP2515_MonitorStack[0x1000];
static uint8_t MCP2515_SndStack[0x1000];

typedef enum
{
    CAN_STD=(unsigned char)(0x00),
    CAN_RTR=(unsigned char)(0x40),
  	CAN_EXD=(unsigned char)(0x80)
}CAN_FrameType_TypeDef;
typedef struct Frame_Strut
{
    unsigned int ID;
    unsigned char DLC;
    CAN_FrameType_TypeDef Type;
}Frame_TypeDef;



struct ShellCmdTab const shell_cmd_mcp2515_table[]=
{
	{
			"mcp2515_rst",
			(bool_t(*)(char*))MCP2515_Reset,
			"复位mcp2515芯片",
			"COMMAND:reset+enter"
	},

};


#define CN_MCP2515_SHELL_NUM  sizeof(shell_cmd_mcp2515_table)/sizeof(struct ShellCmdTab)
static struct ShellCmdRsc tg_mcp2515_shell_cmd_rsc[CN_MCP2515_SHELL_NUM];

/*******************************************************************************
功能:CAN控制器操作shell模块
参数:无.
返回值:1。
*********************************************************************************/
ptu32_t MCP2515_Shell_Module_Install(void)
{
	Sh_InstallCmd(shell_cmd_mcp2515_table,tg_mcp2515_shell_cmd_rsc,CN_MCP2515_SHELL_NUM);
	return 1;
}



//=====================================================================
//函数名：片选拉低
//参数：
//返回值：
//功能：片选拉低，选中器件
//=====================================================================
static void MCP2515_CsActive(void)
{
	 SPI_CsActive(&s_MCP2515_Dev,CN_TIMEOUT_FOREVER);
}



//=====================================================================
//函数名：片选拉高
//参数：
//返回值：
//功能：片选拉高，结束操作
//=====================================================================
static void MCP2515_CsInActive(void)
{
	SPI_CsInactive(&s_MCP2515_Dev);
}



//=====================================================================
//函数名：SPI发送接收函数
//参数：发送的数据地址、数据长度、接受的数据地址、数据长度、偏移量（发送几个数之后再开始发一个收一个，前面是空读）
//返回值：
//功能：SPI发送接收
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

    result = SPI_Transfer(&s_MCP2515_Dev,&data,true,s_MCP2515_Timeout);
    if(result != CN_SPI_EXIT_NOERR)
        return 0;
    return 1;
}



//=====================================================================
//函数名：SPI发送
//参数：
//返回值：
//功能：单独发送想发的数据，不接收
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
//函数名：写寄存器
//参数：寄存器地址和想写入的数据
//返回值：
//功能：通过SPI向MCP2515发送一个字节将返回所需的状态
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
//函数名：读寄存器指令
//参数：想读的寄存器地址
//返回值：
//功能：拉低电平，按顺序发送读指令、目标地址，再读一个数据
//=====================================================================
uint8_t MCP2515_ReadByte(uint8_t Addr)
{
	u8 temp[3],i;
    u8 Data[3];
    temp[0]=Mcp2515_SPI_READ;
    temp[1]=Addr;
    temp[2]=0xff;

    MCP2515_CsActive();

    for(i=0;i<3;i++)
    {
    	MCP2515_TxRx(temp+i,1,Data+i,1,0);
    }

    MCP2515_CsInActive();
    return Data[2];
}



/**
  * @brief  Reset the MCP2515 then U go into the configruation mode
  * @param  none
  * @retval none
  */
//=====================================================================
//函数名：复位指令
//参数：
//返回值：
//功能：复位指令可以重新初始化MCP2515 的内部寄存器，并设置配置模式，上电初始化过程一部分
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
//函数名：修改寄存器的位
//参数：寄存器地址、屏蔽字节、数据字节
//返回值：
//功能：屏蔽字节中为1的位置会修改为数据字节中对应位置的值，屏蔽字节中为0的位置不会变化，手册里说的很详细
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
//函数名：发送指令函数
//参数：指令
//返回值：
//功能：给芯片发送一个指令（最简单的读写功能），写一个，空读一个，再写一个0，再读出返回值
//=====================================================================
uint8_t SPI_CMD_MCP2515(uint8_t CMD)
{
    uint8_t Data;
    MCP2515_CsActive();
    MCP2515_TxRx(&CMD,1,&Data,1,1);
    MCP2515_CsInActive();
    return Data;
}



//----配置CAN通信波特率---------------------------------------------------------
//功能: 配置CAN通信波特率。
//参数:baudrate,CAN通信波特率。
//返回: 成功返回true,失败返回false.
//说明：
//-----------------------------------------------------------------------------
 bool_t MCP2515__SetBaudRate(u8 baudrate)
{
 /* 我来介绍一下波特率的设置方法：一共两个公式：
	 TQ=2(BRP+1)/Fosc
		 波特率=1/(同步段长+传播段长+PS1长+PS2长)*TQ
		联立求解即可    其中             BRP在CNF1里，值等于（寄存器CNF1里BRP的值+1）         Fosc是振荡器的频率8MHZ ，如此可得TQ
		期望波特率已知，TQ已知，可求出四段长度之和，
		同步段长=1   传播段长=（寄存器CNF2里PRSEG的值+1）   PS1长=（寄存器CNF2里PHSEG1的值+1）   PS2长=（寄存器CNF3里PHSEG2的值+1）

		时间段主要满足一个公式：采样点在60%-70%处，采样点在PS1末尾，
		即    (同步段长+传播段长+PS1长)/(同步段长+传播段长+PS1长+PS2长)=60%-70%，例程里面是62.5%
		差不多就是这样
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
	 //设置CNF2，SAM = 0，一次倒谱，PHSEG1 =（2 + 1）TQ = 3TQ，PRSEG =（0 + 1）TQ = 1TQ
	   MCP2515_WriteReg(Mcp2515_CNF2,0x80|Mcp2515_PHSEG1_3TQ|Mcp2515_PRSEG_1TQ);
	 //设置CNF3，PHSEG2 =（2 + 1）TQ = 3TQ，当CANCTRL.CLKEN = 1时，将CLKOUT引脚设置为输出
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
//函数名：发送数据
//参数：想通过CAN总线发送出去的数据所在的缓冲区指针，还有一个结构体，填好8字节数据，29位ID，数据长度，帧类型
//返回值：
//功能：读状态、填缓冲区（写到数据字节寄存器里，像数据寄存器一样）、清标志位、发送
//     发送出去的29位ID顺序不对，需在数据处理时进行调整
//=====================================================================
void MCP2515_SendData(uint8_t *CAN_TX_Buf,Frame_TypeDef *Frame )
{
    uint8_t tempdata;
    uint8_t HSID,LSID,SIDLEID,HEID,LEID;
    u32 SID,EID;
    uint8_t tx_ctrl,tx_buf_addr,tx_cmd,reg_sidh,reg_sidl,reg_eidh,reg_eidl,reg_dlc;
    //Select an empty tx buffer//为0说明空闲
    if((MCP2515_ReadByte(Mcp2515_TXB0CTRL)&Mcp2515_TXREQ)==0)//TXBnCTRL的3号位：TXREQ：报文发送请求位
    {
    	tx_ctrl=Mcp2515_TXB0CTRL;
    	tx_buf_addr=Mcp2515_TXB0D0;//发送缓冲区数据写入的寄存器地址选为TXB0的
    	tx_cmd=Mcp2515_SPI_RTS_TXB0;//发送报文指令选为TXB0的
    	reg_sidh=Mcp2515_TXB0SIDH;
		reg_sidl=Mcp2515_TXB0SIDL;
		reg_eidh=Mcp2515_TXB0EID8;
		reg_eidl=Mcp2515_TXB0EID0;
		reg_dlc=Mcp2515_TXB0DLC;
    }
    else if((MCP2515_ReadByte(Mcp2515_TXB1CTRL)&Mcp2515_TXREQ)==0)
    {
    	tx_ctrl=Mcp2515_TXB1CTRL;
    	tx_buf_addr=Mcp2515_TXB1D0;//发送缓冲区数据写入的寄存器地址选为TXB1的
    	tx_cmd=Mcp2515_SPI_RTS_TXB1;//发送报文指令选为TXB1的
    	reg_sidh=Mcp2515_TXB1SIDH;
		reg_sidl=Mcp2515_TXB1SIDL;
		reg_eidh=Mcp2515_TXB1EID8;
		reg_eidl=Mcp2515_TXB1EID0;
		reg_dlc=Mcp2515_TXB1DLC;
    }
    else if((MCP2515_ReadByte(Mcp2515_TXB2CTRL)&Mcp2515_TXREQ)==0)
	{
    	tx_ctrl=Mcp2515_TXB2CTRL;
    	tx_buf_addr=Mcp2515_TXB2D0;//发送缓冲区数据写入的寄存器地址选为TXB2的
    	tx_cmd=Mcp2515_SPI_RTS_TXB2;//发送报文指令选为TXB2的
    	reg_sidh=Mcp2515_TXB2SIDH;
		reg_sidl=Mcp2515_TXB2SIDL;
		reg_eidh=Mcp2515_TXB2EID8;
		reg_eidl=Mcp2515_TXB2EID0;
		reg_dlc=Mcp2515_TXB2DLC;
	}
    else
    {
    	printf("No available tx buffer.\r\n");
    	return;
    }


    if(Frame->Type==CAN_STD)//见手册CAN报文帧——STD代表标准数据帧
    {
        /*Set the ID of the frame*//*设置帧的ID */
        HSID=(uint8_t)(Frame->ID>>3);//向SID H和L 寄存器写入11位ID值（分别写入10:3 2:0位，所以要移一下位）
        LSID=(uint8_t)((Frame->ID<<5)&0xE0);//左移五位，剩下三位就在8位里的高字节
        MCP2515_WriteReg(reg_sidh,HSID);
        MCP2515_WriteReg(reg_sidl,LSID);
        /*Set the DLC and the type of the frame*//*设置DLC和帧的类型*/
        MCP2515_WriteReg(reg_dlc,Frame->DLC|CAN_STD);//设置数据长度，该寄存器里还有一个RTR位
        /*Write the data into the TXB0 data registers */
        for(tempdata=0;tempdata<Frame->DLC;tempdata++)
        {
            MCP2515_WriteReg(tx_buf_addr+tempdata,CAN_TX_Buf[tempdata]);
        }
    }
    else if(Frame->Type==CAN_RTR) /*if(CAN_FrameType==CAN_RTR)*///RTR
    {
        /*Set the ID of the frame*/
        HSID=(uint8_t)(Frame->ID>>3);
        LSID=(uint8_t)((Frame->ID<<5)&0xE0);
        MCP2515_WriteReg(Mcp2515_TXB0SIDH,HSID);
        MCP2515_WriteReg(Mcp2515_TXB0SIDL,LSID);
        /*Set the type of the frame*/
        MCP2515_WriteReg(Mcp2515_TXB0DLC,CAN_RTR);
    }
    else if(Frame->Type==CAN_EXD)//发送扩展帧
    {
    	SID=Frame->ID>>18;//扩展帧中基本ID代表了29位标识符的高11位
    	SID&=0x7ff;
    	HSID=(uint8_t)(SID>>3);//先右移18位的值就是高11位
    	LSID=(uint8_t)((SID<<5)&0xE0);

    	EID=Frame->ID &0x3FFFF;//剩下EID中的18位
    	SIDLEID=(uint8_t)(EID>>16);//SIDL中的2位
    	HEID=(uint8_t)(EID>>8);//右移8位之后强制转换应该可以得到15:8位
    	LEID=(uint8_t)EID;//直接强制转换去掉高位，剩下8位就是低8位

        MCP2515_WriteReg(reg_sidh,HSID);
        MCP2515_WriteReg(reg_sidl,LSID | Mcp2515_EXIDE_SET | SIDLEID);//扩展使能以及EID高两位

        MCP2515_WriteReg(reg_eidh,HEID);
        MCP2515_WriteReg(reg_eidl,LEID);

        MCP2515_WriteReg(reg_dlc,Frame->DLC);//设置数据长度，该寄存器里还有一个RTR位

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

    Djy_EventDelay(10*mS);
    tempdata=SPI_CMD_MCP2515(Mcp2515_SPI_READ_STATUS);
	if(0==(tempdata & 0x54))
	{
		gs_u64HardSndCnt++;
	}
	else
	{
		gs_u64SndPkgBadCnt++;
		//手动清发送状态标志位
		MCP2515_BitModify(tx_ctrl,Mcp2515_TXREQ,0x00);
	}
}



/**
  * @brief  Receive n bytes from MCP2515 RXB0
  * @param  none
  * @retval Data:return the effectively data from RXB0
  */
//=====================================================================
//函数名：接收数据
//参数：存放从寄存器里取出的数据的地址，报文帧
//返回值：
//功能：读状态、读各种数据,这里只设计了扩展帧报文的解析
//=====================================================================
void MCP2515_ReceiveData(void)
{
    uint8_t tempdata,i;
    uint8_t buf[13];
    u32 CAN_ID;
    uint8_t HSID,LSID,SIDLEID,HEID,LEID,DLC;
    u32 SID,EID;
    uint8_t CAN_RX_Buf[8];
    Frame_TypeDef *Frame;

    tempdata=SPI_CMD_MCP2515(Mcp2515_SPI_RX_STATUS);//RX状态指令看接收到的报文的缓冲器、类型、滤波器
                                                    //是写完指令再读呢还是写一个读一个？
    if(tempdata&0x40)//RXB0中收到报文，标志位为1
    {
    	HSID=MCP2515_ReadByte(Mcp2515_RXB0SIDH);//把29位标识符都读出来
		LSID=MCP2515_ReadByte(Mcp2515_RXB0SIDL);//从这四个寄存器里读
		HEID=MCP2515_ReadByte(Mcp2515_RXB0EID8);
		LEID=MCP2515_ReadByte(Mcp2515_RXB0EID0);

		if(LSID |Mcp2515_EXIDE_SET)//说明是扩展帧，这个寄存器里有个扩展标识符标志位
		{
			Frame->Type=CAN_EXD;

			//把这四个寄存器里的数据处理一下，移个位，得到正确的29位ID
			SID=LSID>>5;//SID2:0移至最右
			SID|=HSID<<3;//SID10:3左移三位与SID22:0合并

			SIDLEID=LSID & 0x03;
			EID=LEID;//EID低8位
			EID|=(HEID<<8);//与EID高8位合并
			EID|=(SIDLEID<<16);//与EID17:16位合并

			CAN_ID=EID|(SID<<18);//得到最后ID

		}
		else//标准帧，先不管
		{
		}
		Frame->ID=CAN_ID;//收到的ID给结构体
		DLC=MCP2515_ReadByte(Mcp2515_RXB0DLC);//读一下有几个数据，准备处理后8字节数据
		DLC=(DLC & 0x0F);
		Frame->DLC=DLC;
		for(tempdata=0;tempdata<DLC;tempdata++)//把数据保存至缓冲区
		{
			CAN_RX_Buf[tempdata]=MCP2515_ReadByte(Mcp2515_RXB0D0+tempdata);
		}
		MCP2515_BitModify(Mcp2515_CANINTF,Mcp2515_RX0IF_SET,0x00);//一旦缓冲器中的报文处理完毕，必须将该位清零以接收下一条报文


        buf[0]=(uint8_t)(Frame->DLC|Frame->Type);
		for(i=4;i>0;i--)
		{
			buf[i]=(Frame->ID & 0xff);
			Frame->ID=(Frame->ID>>8);
		}
		for(i=0;i<DLC;i++)
		{
			buf[5+i]=CAN_RX_Buf[i];
		}
		Ring_Write(gs_MCP2525_RcvRing,buf,5+DLC);
		gs_u64HardRcvCnt++;
    }
//=============================================================================
    //一般RXB0就能胜任，不会用到
//=============================================================================
    if(tempdata&0x80)//RXB1中收到报文
    {
    	HSID=MCP2515_ReadByte(Mcp2515_RXB1SIDH);
		LSID=MCP2515_ReadByte(Mcp2515_RXB1SIDL);
		HEID=MCP2515_ReadByte(Mcp2515_RXB1EID8);
		LEID=MCP2515_ReadByte(Mcp2515_RXB1EID0);

		if(LSID |Mcp2515_EXIDE_SET)//扩展帧
		{
			Frame->Type=CAN_EXD;

			SID=LSID>>5;//SID2:0移至最右
			SID|=HSID<<3;//SID10:3左移三位与SID22:0合并

			SIDLEID=LSID & 0x03;
			EID=LEID;//EID低8位
			EID|=(HEID<<8);//与EID高8位合并
			EID|=(SIDLEID<<16);//与EID17:16位合并

			CAN_ID=EID|(SID<<18);//得到最后ID

		}
		else//标准帧，先不管
		{
		}


		Frame->ID=CAN_ID;//收到的ID给结构体
		Djy_EventDelay(1*mS);
		DLC=MCP2515_ReadByte(Mcp2515_RXB1DLC);
		DLC=(DLC & 0x0F);
		Frame->DLC=DLC;
		for(tempdata=0;tempdata<DLC;tempdata++)
		{
			CAN_RX_Buf[tempdata]=MCP2515_ReadByte(Mcp2515_RXB1D0+tempdata);
		}
		MCP2515_BitModify(Mcp2515_CANINTF,Mcp2515_RX1IF,0x00);//一旦缓冲器中的报文处理完毕，必须将该位清零以接收下一条报文


	   buf[0]=(uint8_t)(Frame->DLC|Frame->Type);
	   for(i=4;i>0;i--)
	  {
		buf[i]=(Frame->ID & 0xff);
		Frame->ID=(Frame->ID>>8);
	  }
	  for(i=0;i<DLC;i++)
	  {
		buf[5+i]=CAN_RX_Buf[i];
	  }
	  Ring_Write(gs_MCP2525_RcvRing,buf,5+DLC);
	  gs_u64HardRcvCnt++;
    }

}



//=====================================================================
//函数名：报文错误处理函数
//参数：
//返回值：
//功能：
//=====================================================================
void __MCP2515_MsgErrHandle(void)
{

}



//=====================================================================
//函数名：错误中断处理函数
//参数：
//返回值：
//功能：
//=====================================================================
void __MCP2515_ErrHandle(void)
{

}



//=====================================================================
//函数名：hal库GPIO口外部中断回调函数
//参数：
//返回值：
//功能：中断调用HAL_GPIO_EXTI_IRQHandler后，清标志位，然后调用本回调函数
//     本函数调用CAN2_Handle实现mcp2515接收报文测试
//=====================================================================
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	u8 flag,eflg,tec,rec;
	Int_ClearLine(CN_INT_LINE_EXTI3);
    switch(GPIO_Pin)
    {
      case GPIO_PIN_3:
    	  flag=MCP2515_ReadByte(Mcp2515_CANINTF);
    	  if(flag&Mcp2515_MERRF)
    	  {
    		  MCP2515_BitModify(Mcp2515_CANINTF,Mcp2515_MERRF,0x00);
    	  }

    	  else if(flag&Mcp2515_ERRIF)
    	  {
    		  eflg=MCP2515_ReadByte(Mcp2515_EFLG);
    		  tec=MCP2515_ReadByte(Mcp2515_TEC);
    		  rec=MCP2515_ReadByte(Mcp2515_REC);

    		  MCP2515_BitModify(Mcp2515_CANINTF,Mcp2515_ERRIF,0x00);
    	  }

    	  else if(flag&(Mcp2515_RX1IF | Mcp2515_RX0IF))
    	  {
    		  MCP2515_ReceiveData();
    	  }
        break;
    }
}



//=====================================================================
//函数名：外部中断初始化函数
//参数：
//返回值：
//功能：
//=====================================================================
void  MCP2515_IntInit(void)
{
	Int_Register(MCP2515_EXIT_INT_LINE);
	Int_IsrConnect(MCP2515_EXIT_INT_LINE,(void(*))HAL_GPIO_EXTI_IRQHandler);//中断触发会调用该函数（位于hal_gpio.c）
	Int_SetIsrPara(MCP2515_EXIT_INT_LINE,GPIO_PIN_3);//参数就是引脚3（外部中断相同引脚号的gpio口共用一个中断）
	Int_SettoAsynSignal(MCP2515_EXIT_INT_LINE);
	Int_ClearLine(MCP2515_EXIT_INT_LINE);
	Int_RestoreAsynLine(MCP2515_EXIT_INT_LINE);
}



//=====================================================================
//函数名：上层调用发送报文函数
//参数：发送缓冲区，长度
//返回值：
//功能：
//=====================================================================
uint32_t MCP2515_WriteData(uint8_t *txbuf,uint32_t len)
{
	uint8_t pkgnum,i,j;
	bool_t ret;
	uint8_t buf[13];
	//参数检查
	if(txbuf==NULL)
	{
	   printf("Function:%s para invailed.\r\n",__FUNCTION__);
	   return 0;
	}

	if((len%13!=0)||(len==0))
	{
		printf("Function:%s para invailed.\r\n",__FUNCTION__);
		return 0;
	}
	pkgnum=len/13;
	gs_u64AppSndCnt+=pkgnum;
	for(i=0;i<pkgnum;i++)
	{
		for(j=0;j<13;j++)
		 {
			buf[j]=txbuf[13*i+j];
		 }
		 ret=MsgQ_Send(gs_ptCanSndMsgQ,buf,CN_MCP2515_MSGQ_LEN,0, CN_MSGQ_PRIO_NORMAL);
		 if(!ret)
		 {
			printf("CAN Snd MsgQ is full.\r\n");
		 }
	}
	return len;

}



//=====================================================================
//函数名：上层调用读取报文函数
//参数：接收缓冲区，长度
//返回值：
//功能：
//=====================================================================
uint32_t MCP2515_ReadData(uint8_t* rxBuf,uint32_t len)
{
	uint32_t rdLen=0x0000,ringlen,pkgnum,r_len;
	atom_low_t atom;
	//先作参数检查
	if(rxBuf==NULL)
		 return 0;
	atom = Int_LowAtomStart();
	ringlen=Ring_Check(gs_MCP2525_RcvRing);
	Int_LowAtomEnd(atom);
	if(len>ringlen)
	{
		r_len=ringlen;
	}
	else
	{
		r_len=len;
	}
	rdLen=Ring_Read(gs_MCP2525_RcvRing,rxBuf,r_len);
	pkgnum=rdLen/13;
	gs_u64AppRcvCnt+=pkgnum;
	return rdLen;
}



//----MCP2515发送线程---------------------------------------------------------
//功能: 异步发送CAN数据，判断是否发送消息队列中是否有数据，如果有数据则将数据发送出去。
//参数: 无.
//返回: 无.
//-----------------------------------------------------------------------------
ptu32_t __MCP2515_SndTask(void)
{
      uint8_t buf[13];
      Frame_TypeDef frame;
      uint32_t Id;
      while(1)
      {
         MsgQ_Receive(gs_ptCanSndMsgQ, buf, CN_MCP2515_MSGQ_LEN, CN_TIMEOUT_FOREVER);
		 Id=0x0000;
		 Id|=(uint32_t)buf[1]<<24;
		 Id|=(uint32_t)buf[2]<<16;
		 Id|=(uint32_t)buf[3]<<8;
		 Id|=(uint32_t)buf[4];
		 frame.DLC=8;
		 frame.ID=Id;
		 frame.Type=CAN_EXD;
		 MCP2515_SendData(&(buf[5]),&frame);
      }
}


//=====================================================================
//函数名：初始化
//参数：
//返回值：
//功能：复位指令可以重新初始化MCP2515 的内部寄存器，并设置配置模式，上电初始化过程一部分
//=====================================================================
void MCP2515_Init(u8  baudrate)
{
    uint8_t dummy=0;
        /*首先初始化SPI然后初始化MCP2515
        1>通过复位MCP2515进入配置模式或写入CANCTRL.REQOP的位。
        2>如果MCP2515已经进入配置模式，请通过MCP2515_ReadByte（CANSTAT）检查CANSTAT。
        3>配置寄存器设置波特率：CN1，CN2，CN3。
        4>配置TXRTSCTRL来设置TXnRTS引脚的功能。
        5>配置TRXBnCTRL设置发送邮箱的功能：TXB0，TXB1，TXB2。
        6>配置TXBn'SID，EID和DLC。这个步骤可以在正常模式下进行配置。
        7>配置RXFnSIDH和RXFnSIDL来获取特定的ID。
        8>配置RXMnSIDH和RXMnSIDL来屏蔽RXFnSID的ID。
        9>配置CANINTE来启用或禁用中断。
        10>返回到正常模式，再次检查CANSTAT是否返回正常模式。
        */
    extern const Pin CAN_RstPin[];
    PIO_Set(&CAN_RstPin[0]);
    Djy_EventDelay(1*mS);
    PIO_Clear(&CAN_RstPin[0]);
    Djy_EventDelay(1*mS);
    PIO_Set(&CAN_RstPin[0]);
    Djy_EventDelay(1*mS);//硬件复位

	/*翻到配置模式*/
    MCP2515_Reset();//软件复位
    Djy_EventDelay(10*mS);
    dummy=MCP2515_ReadByte(Mcp2515_CANSTAT);

    if(4!=(dummy>>5))
    {
    	printf("未进入配置状态.\r\n");
    	return;
    }
    Djy_EventDelay(1*mS);

	MCP2515__SetBaudRate(baudrate);
    /*
         3 传输缓冲区：TXB0，TXB1，TXB2
         2 接收缓冲区：RXB0，RXB1
         6 ID过滤器：RXF0〜RXF5
         2 ID掩码寄存器：RXM0，RXM1
    */
    //配置RXB0'寄存器
    MCP2515_WriteReg(Mcp2515_RXB0CTRL,0x60);//接收来自CAN总线的所有帧
    MCP2515_WriteReg(Mcp2515_RXB1CTRL,0x60);//接收来自CAN总线的所有帧

    MCP2515_WriteReg(Mcp2515_RXM0SIDH,0x00);//Mask register//屏蔽寄存器
    MCP2515_WriteReg(Mcp2515_RXM0SIDL,0x00);//不对所有位置滤波
	MCP2515_WriteReg(Mcp2515_RXM1SIDH,0x00);//Mask register//屏蔽寄存器
    MCP2515_WriteReg(Mcp2515_RXM1SIDL,0x00);//不对所有位置滤波

     /*Set the RXB0 or RXB1 interrupt enableling 设置RXB0或RXB1中断使能*/
     //启用RXB0，RXB1中断
     //启用错误中断，报文错误中断
     MCP2515_WriteReg(Mcp2515_CANINTE,Mcp2515_RX0IE|Mcp2515_RX1IE|Mcp2515_ERRIE|Mcp2515_MERRE);

     MCP2515_BitModify(Mcp2515_CANINTF,Mcp2515_RX0IF,0x00);//标志位清零
     MCP2515_BitModify(Mcp2515_CANINTF,Mcp2515_RX1IF,0x00);
     MCP2515_BitModify(Mcp2515_CANINTF,Mcp2515_ERRIF,0x00);//标志位清零
     MCP2515_BitModify(Mcp2515_CANINTF,Mcp2515_MERRF,0x00);


//     MCP2515_WriteReg(Mcp2515_RXB0CTRL,0x20);// RXB0只是接收标准帧
//     MCP2515_WriteReg(Mcp2515_RXF0SIDH,0xFF);//Fileter register//过滤器寄存器
//     MCP2515_WriteReg(Mcp2515_RXF0SIDL,0xE0);//Just receive ID=0x7FF frame//只需接收ID = 0x7FF帧
//     MCP2515_WriteReg(Mcp2515_RXF1SIDH,0xFD);//Fileter register//过滤器寄存器
//     MCP2515_WriteReg(Mcp2515_RXF1SIDL,0xC0);//Just receive ID=0x7EE frame//只需接收ID = 0x7EE帧

//     Configurate TXB0's ID and DLC registers//配置TXB0的ID和DLC寄存器
//     MCP2515_WriteReg(Mcp2515_TXB0SIDH,0xFF);//Standard ID//标准ID
//     MCP2515_WriteReg(Mcp2515_TXB0SIDL,0xE0);//Standard ID//标准ID
//     MCP2515_WriteReg(Mcp2515_TXB0DLC,Mcp2515_DLC_1);//DLC

     MCP2515_WriteReg(Mcp2515_CANCTRL,Mcp2515_REQOP_NORMAL | Mcp2515_CLKOUT_ENABLED);//进入正常模式
     dummy=MCP2515_ReadByte(Mcp2515_CANSTAT);
        if (Mcp2515_OPMODE_NORMAL != (dummy & 0xE0))
          MCP2515_WriteReg(Mcp2515_CANCTRL,Mcp2515_REQOP_NORMAL | Mcp2515_CLKOUT_ENABLED);//If normal ok

        if(0==dummy>>5)
        {
        	printf("mode normal.\r\n");
        }
}



//----将MCP2515设备挂接到SPIBUS上---------------------------------------------------------
//功能:
//参数: BusName：总线名称
//
//返回: 成功初始化则返回true,失败则返回false.
//-----------------------------------------------------------------------------
bool_t ModuleInstall_MCP2515(char *BusName)
{
	uint16_t evtt_id,event_id;
	struct RingBuf *RingBuf;
	if(NULL == Lock_SempCreate_s(&s_MCP2515_Semp,1,1,CN_BLOCK_FIFO,"MCP2515_semp"))
	        return false;
	    /* setup baud rate */
	s_MCP2515_Dev.AutoCs = false;
	s_MCP2515_Dev.CharLen = 8;
	s_MCP2515_Dev.Cs = 0;
	s_MCP2515_Dev.Freq = MCP2515_SPI_SPEED;
	s_MCP2515_Dev.Mode = SPI_MODE_0;
	s_MCP2515_Dev.ShiftDir = SPI_SHIFT_MSB;

	if(NULL != SPI_DevAdd_s(&s_MCP2515_Dev,BusName,"MCP2515"))
	{
		SPI_BusCtrl(&s_MCP2515_Dev,CN_SPI_SET_POLL,0,0);
		sMCP2515Inited=true;
	}
	MCP2515_Shell_Module_Install();
	gs_MCP2525_RcvRing=Ring_Create(CN_MCP2515_RCV_LEN);
	if(gs_MCP2525_RcvRing==NULL)
	{
		printf("%s create ring failed.\r\n",__FUNCTION__);
		return false;
	}

	 gs_ptCanSndMsgQ=MsgQ_Create(CN_MCP2515_MSGQ_NUM,CN_MCP2515_MSGQ_LEN,CN_MSGQ_TYPE_FIFO);
	 if(gs_ptCanSndMsgQ==NULL)
			return false;

/*
	 evtt_id = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,__MCP2515_Monitor,
	 			  MCP2515_MonitorStack,sizeof(MCP2515_MonitorStack),"MCP2515 Monitor function");
	 if(evtt_id!=CN_EVTT_ID_INVALID)
	 {
	   event_id=Djy_EventPop(evtt_id,NULL,0,NULL,0,0);
	 }
	 else
	 {
		Djy_EvttUnregist(evtt_id);
		 printf("MCP2515 Monitor evtt pop failed.\r\n");
	 }
	 */
	evtt_id = Djy_EvttRegist(EN_CORRELATIVE,100,0,0,__MCP2515_SndTask,
			  MCP2515_SndStack,sizeof(MCP2515_SndStack),"MCP2515 Snd Task");
	 if(evtt_id!=CN_EVTT_ID_INVALID)
	 {
		 event_id=Djy_EventPop(evtt_id,NULL,0,NULL,0,0);
	 }
	 else
	{
		Djy_EvttUnregist(evtt_id);
		printf("CAN Monitor  evtt pop failed.\r\n");
	}
	printf("CAN install OK.\r\n");


	return sMCP2515Inited;
}
















