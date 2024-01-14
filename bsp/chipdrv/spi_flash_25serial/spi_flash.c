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

#include <string.h>
#include <stdlib.h>
#include <cpu_peri.h>
#include <djyos.h>
#include <device.h>
#include <filesystems.h>
#include <device/djy_flash.h>
#include <spibus.h>
#include <unit_media.h>
#include <dbug.h>
#include "spi_flash.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern s32 ModuleInstall_SpiFlash(void);
//    ModuleInstall_SpiFlash();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"spi flash"    //SPI��QSPI�ӿڵ�flashоƬ
//parent:"none"//��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"spi bus",//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"       //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_NORFLASH_SPI_FLASH == false )
//#warning  " spi flash  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_NORFLASH_SPI_FLASH    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,true,false,
#define CFG_SPI_FLASH_PART_ERASE               false      //"����ѡ��,�Ƿ���Ҫ������оƬ��"
#define CFG_SPI_FLASH_QSPI_ENABLE              false      //"�Ƿ�ʹ��QSPIģʽ"
//%$#@string,1,10,
#define CFG_SPI_FLASH_SPI_NAME                 "SPI1"      //"�õ�SPI�豸���֣�spiģʽ����Ч"
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

#if CFG_SPI_FLASH_QSPI_ENABLE == FALSE
static struct SPI_Device *s_ptSpiPort; // ����ʹ�õ�SPI�˿�
#endif
struct MutexLCB *SpiFlashLock;           //оƬ������ʱ���
struct NorDescr *SpiFlashdescription = NULL;
bool_t SpiFlashInitFlag = false;
const char *SpiFlashName = "spi flash dev";      //��flash��obj�ڵ�����
struct umedia *SpiFlashUm;
extern struct Object *s_ptDeviceRoot;
static bool_t Enable4B_AddrOpt = false;


static u32 FlashPageSize = 0;        //"ÿҳ���ֽ���"
static u32 FlashPageInSector = 0;         //"ÿ�������ж���ҳ"
static u32 FlashSectorInBolck = 0;         //"ÿ�����ж�������"
static u32 FlashSectorNum =0;       //"�ܵ�������"


#define CMD_SpiFlashWriteEnable        0x06
#define CMD_SpiFlashWriteDisable       0x04
#define CMD_SpiFlashReadStatusReg1     0x05
#define CMD_SpiFlashReadStatusReg2     0x35
#define CMD_SpiFlashReadStatusReg3     0x15
#define CMD_SpiFlashWriteStatusReg1    0x01
#define CMD_SpiFlashWriteStatusReg2    0x31
#define CMD_SpiFlashWriteStatusReg3    0x11
#define CMD_SpiFlashReadData           0x03
#define CMD_SpiFlashReadData_4B_Addr   0x13
#define CMD_SpiFlashFastReadData       0x0B
#define CMD_SpiFlashFastReadDual       0x3B
#define CMD_SpiFlashPageProgram        0x02
#define CMD_SpiFlashPageProgram_4B_Addr 0x12
#define CMD_SpiFlashBlockErase         0xD8
#define CMD_SpiFlashBlockErase_4B_Addr 0xDC
#define CMD_SpiFlashSectorErase        0x20
#define CMD_SpiFlashSectorErase_4B_Addr 0x21
#define CMD_SpiFlashChipErase          0xC7
#define CMD_SpiFlashPowerDown          0xB9
#define CMD_SpiFlashReleasePowerDown   0xAB
#define CMD_SpiFlashDeviceID           0xAB
#define CMD_SpiFlashManufactDeviceID   0x90
#define CMD_SpiFlashJedecDeviceID      0x9F
#define CMD_SpiFlashEnable4ByteAddr    0xB7
#define CMD_SpiFlashExit4ByteAddr      0xE9
#define CMD_SpiFlashSetReadParam       0xC0
#define CMD_SpiFlashEnterQPIMode       0x38
#define CMD_SpiFlashExitQPIMode        0xFF

static struct SPI_FlashInfo  FlashInfo[] ={
    {"W25Q64",              0xEF4017, 256, 16, 16, 8 * 1024 * 1024},
    {"W25Q128",             0xEF4018, 256, 16, 16, 16 * 1024 * 1024},
    {"W25Q256",             0xEF4019, 256, 16, 16, 32 * 1024 * 1024},

//  =======================��δ��֤==========================================
    // {"W25X10",              0xEF3011, 256, 16, 16, 128 * 1024},
    // {"W25X20",              0xEF3012, 256, 16, 16, 256 * 1024},
    // {"W25X40",              0xEF3013, 256, 16, 16, 512 * 1024},
    // {"W25X80",              0xEF3014, 256, 16, 16, 1 * 1024 * 1024},
    // {"W25X16",              0xEF3015, 256, 16, 16, 2 * 1024 * 1024},
    // {"W25X32",              0xEF3016, 256, 16, 16, 4 * 1024 * 1024},
    // {"W25X64",              0xEF3017, 256, 16, 16, 8 * 1024 * 1024},
    // {"W25X128",             0xEF3018, 256, 16, 16, 16 * 1024 * 1024},
    // {"W25Q40",              0xEF4013, 256, 16, 16, 512 * 1024},
    // {"W25Q80",              0xEF4014, 256, 16, 16, 1 * 1024 * 1024},
    // {"W25Q16",              0xEF4015, 256, 16, 16, 2 * 1024 * 1024},
    // {"W25Q32",              0xEF4016, 256, 16, 16, 4 * 1024 * 1024},
    // {"M25P040",             0x514013, 256, 16, 16, 512 * 1024},
    // {"AT25DF321",           0xC2201A, 256, 16, 16, 100 * 1024 * 1024},
    // {"MX25L3205",           0xC22016, 256, 16, 16, 4 * 1024 * 1024},
    // {"A25L05PU",            0x372010, 256, 16, 16, 64 * 1024},
    // {"A25L05PT",            0x372020, 256, 16, 16, 64 * 1024},
    // {"A25L10PU",            0x372011, 256, 16, 16, 128 * 1024},
    // {"A25L05PT",            0x372021, 256, 16, 16, 128 * 1024},
    // {"A25L20PU",            0x372012, 256, 16, 16, 256 * 1024},
    // {"A25L20PT",            0x372022, 256, 16, 16, 256 * 1024},
    // {"A25L512",             0x373010, 256, 16, 16, 64 * 1024},
    // {"A25L010",             0x373011, 256, 16, 16, 128 * 1024},
    // {"A25L020",             0x373012, 256, 16, 16, 256 * 1024},
    // {"A25L40P",             0x372013, 256, 16, 16, 512 * 1024},
    // {"A25L040",             0x373013, 256, 16, 16, 512 * 1024},
    // {"A25L80P",             0x372014, 256, 16, 16, 1 * 1024 * 1024},
    // {"A25L80P",             0x7F3720, 256, 16, 16, 1 * 1024 * 1024},
    // {"A25L080",             0x373015, 256, 16, 16, 1 * 1024 * 1024},
    // {"A25L080",             0x373014, 256, 16, 16, 1 * 1024 * 1024},
    // {"A25L16P(bottom)",     0x372015, 256, 16, 16, 2 * 1024 * 1024},
    // {"A25L16P(otp)",        0x372025, 256, 16, 16, 2 * 1024 * 1024},
    // {"A25L016",             0x373015, 256, 16, 16, 2 * 1024 * 1024},
    // {"A25L032",             0x373016, 256, 16, 16, 4 * 1024 * 1024},
    // {"A25LQ32A",            0x374016, 256, 16, 16, 4 * 1024 * 1024},
    // {"AT25FS010",           0x1F6601, 256, 16, 16, 128 * 1024},
    // {"AT25FS040",           0x1F6604, 256, 16, 16, 512 * 1024},
    // {"AT25F512",            0x1F6500, 256, 16, 16, 64 * 1024},
    // {"AT25F512B",           0x1F6500, 256, 16, 16, 64 * 1024},
    // {"AT25F512A",           0x1F6500, 256, 16, 16, 64 * 1024},
    // {"AT25F1024",           0x1F, 256, 16, 16, 128 * 1024},
    // {"AT25F2048",           0x1F, 256, 16, 16, 256 * 1024},
    // {"AT25F4096",           0x1F, 256, 16, 16, 512 * 1024},
    // {"AT25DF021",           0x1F4300, 256, 16, 16, 256 * 1024},
    // {"AT25DF041A",          0x1F4401, 256, 16, 16, 512 * 1024},
    // {"AT25DF081A",          0x1F4501, 256, 16, 16, 1 * 1024 * 1024},
    // {"AT25DF161A",          0x1F4602, 256, 16, 16, 2 * 1024 * 1024}
    // {"AT25DF321",           0x1F4700, 256, 16, 16, 4 * 1024 * 1024},
    // {"AT25DF321A",          0x1F4701, 256, 16, 16, 4 * 1024 * 1024},
    // {"AT25DF641",           0x1F4800, 256, 16, 16, 8 * 1024 * 1024},
    // {"AT26F004",            0x1F0400, 256, 16, 16, 512 * 1024},
    // {"AT26DF041",           0x1F4400, 256, 16, 16, 512 * 1024},
    // {"AT26DF081A",          0x1F4501, 256, 16, 16, 1 * 1024 * 1024},
    // {"AT26DF161A",          0x1F4601, 256, 16, 16, 2 * 1024 * 1024},
    // {"AT26DF161",           0x1F4600, 256, 16, 16, 2 * 1024 * 1024},
    // {"AT26DF321",           0x1F4700, 256, 16, 16, 4 * 1024 * 1024},
    // {"EN25B05",             0x1c2010, 256, 16, 16, 64 * 1024},
    // {"EN25B05T",            0x1c2010, 256, 16, 16, 64 * 1024},
    // {"EN25F05",             0x1c3110, 256, 16, 16, 64 * 1024},
    // {"EN25P05",             0x1c2010, 256, 16, 16, 64 * 1024},
    // {"EN25LF05",            0x1c3110, 256, 16, 16, 64 * 1024},
    // {"EN25F10",             0x1c3111, 256, 16, 16, 128 * 1024},
    // {"EN25P10",             0x1c2011, 256, 16, 16, 128 * 1024},
    // {"EN25LF10",            0x1c3111, 256, 16, 16, 128 * 1024},
    // {"EN25LF20",            0x1c3112, 256, 16, 16, 256 * 1024},
    // {"EN25F20",             0x1c3112, 256, 16, 16, 256 * 1024},
    // {"EN25F40",             0x1c3113, 256, 16, 16, 512 * 1024},
    // {"EN25LF40",            0x1c3113, 256, 16, 16, 512 * 1024},
    // {"EN25Q80",             0x1c3014, 256, 16, 16, 1 * 1024 * 1024},
    // {"EN25P80",             0x1c2014, 256, 16, 16, 1 * 1024 * 1024},
    // {"EN25T80",             0x1c5114, 256, 16, 16, 1 * 1024 * 1024},
    // {"EN25F80",             0x1c3114, 256, 16, 16, 1 * 1024 * 1024},
    // {"EN25D16",             0x1c3015, 256, 16, 16, 2 * 1024 * 1024},
    // {"EN25B16",             0x1c2015, 256, 16, 16, 2 * 1024 * 1024},
    // {"EN25B16T",            0x1c2015, 256, 16, 16, 2 * 1024 * 1024},
    // {"EN25F16",             0x1c3115, 256, 16, 16, 2 * 1024 * 1024},
    // {"EN25Q16",             0x1c3015, 256, 16, 16, 2 * 1024 * 1024},
    // {"EN25QH16",            0x1c7015, 256, 16, 16, 2 * 1024 * 1024},
    // {"EN25T16",             0x1c5115, 256, 16, 16, 2 * 1024 * 1024},
    // {"EN25B32",             0x1c2016, 256, 16, 16, 4 * 1024 * 1024},
    // {"EN25B32T",            0x1c2016, 256, 16, 16, 4 * 1024 * 1024},
    // {"EN25P32",             0x1c2016, 256, 16, 16, 4 * 1024 * 1024},
    // {"EN25F32",             0x1c3116, 256, 16, 16, 4 * 1024 * 1024},
    // {"EN25Q32",             0x1c3316, 256, 16, 16, 4 * 1024 * 1024},
    // {"EN25QH32",            0x1c7016, 256, 16, 16, 4 * 1024 * 1024},
    // {"EN25B64",             0x1c2017, 256, 16, 16, 8 * 1024 * 1024},
    // {"EN25B64T",            0x1c2017, 256, 16, 16, 8 * 1024 * 1024},
    // {"EN25P64",             0x1c2017, 256, 16, 16, 8 * 1024 * 1024},
    // {"EN25Q64",             0x1c3017, 256, 16, 16, 8 * 1024 * 1024},
    // {"EN25QH64",            0x1c7017, 256, 16, 16, 8 * 1024 * 1024},
    // {"EN25Q128",            0x1c3018, 256, 16, 16, 16 * 1024 * 1024},
    // {"ES25P40",             0x4A2013, 256, 16, 16, 256 * 1024},
    // {"ES25P80",             0x4A2014, 256, 16, 16, 1 * 1024 * 1024},
    // {"ES25P16",             0x4A2015, 256, 16, 16, 2 * 1024 * 1024},
    // {"F25L04UA",            0x8C8C8C, 256, 16, 16, 256 * 1024},
    // {"F25L004A",            0x8C2013, 256, 16, 16, 256 * 1024},
    // {"F25L08PA",            0x8C2014, 256, 16, 16, 1 * 1024 * 1024},
    // {"F25L008A",            0x8C2014, 256, 16, 16, 1 * 1024 * 1024},
    // {"F25L016A",            0x8C2015, 256, 16, 16, 2 * 1024 * 1024},
    // {"F25L16PA",            0x8C1414, 256, 16, 16, 2 * 1024 * 1024},
    // {"F25L32PA",            0x8C2016, 256, 16, 16, 4 * 1024 * 1024},
    // {"F25L32QA",            0x8C4016, 256, 16, 16, 4 * 1024 * 1024},
    // {"F25L64QA",            0x8C4117, 256, 16, 16, 8 * 1024 * 1024},
    // {"SST25VF040",          0xBF258d, 256, 16, 16, 512 * 1024},
    // {"GD25Q512",            0xC84010, 256, 16, 16, 64 * 1024},
    // {"GD25Q10",             0xC84011, 256, 16, 16, 128 * 1024},
    // {"GD25Q20B",            0xC84012, 256, 16, 16, 256 * 1024},
    // {"GD25Q21B",            0xC84112, 256, 16, 16, 256 * 1024},
    // {"GD25Q40B",            0xC84013, 256, 16, 16, 512 * 1024},
    // {"GD25Q41B",            0xC84013, 256, 16, 16, 512 * 1024},
    // {"GD25LQ40",            0xC86013, 256, 16, 16, 512 * 1024},
    // {"GD25F40",             0xC82013, 256, 16, 16, 512 * 1024},
    // {"GD25F80",             0xC82014, 256, 16, 16, 1 * 1024 * 1024},
    // {"GD25Q80",             0xC84014, 256, 16, 16, 1 * 1024 * 1024},
    // {"GD25Q16",             0xC84015, 256, 16, 16, 2 * 1024 * 1024},
    // {"GD25Q32B",            0xC84016, 256, 16, 16, 4 * 1024 * 1024},
    // {"GD25Q64",             0xC84017, 256, 16, 16, 8 * 1024 * 1024},
    // {"GD25Q128",            0xC84018, 256, 16, 16, 16 * 1024 * 1024},
    // {"GD25Q256",            0xC84019, 256, 16, 16, 32 * 1024 * 1024},
    // {"GD25Q512",            0xC84020, 256, 16, 16, 64 * 1024 * 1024},
    // {"KH25L4006E",          0xC2013 , 256, 16, 16, 512 * 1024},
    // {"KH25L8006E",          0xC2014 , 256, 16, 16, 1 * 1024 * 1024},
    // {"KH25L8036",           0xC22615, 256, 16, 16, 1 * 1024 * 1024},
    // {"KH25L1606E",          0xC2015 , 256, 16, 16, 2 * 1024 * 1024},
    // {"MS25X40",             0x373013, 256, 16, 16, 512 * 1024},
    // {"MX25V512",            0xC22010, 256, 16, 16, 64 * 1024},
    // {"MX25L512",            0xC22010, 256, 16, 16, 64 * 1024},
    // {"MX25V1005",           0xC22011, 256, 16, 16, 128 * 1024},
    // {"MX25V2005",           0xC22012, 256, 16, 16, 256 * 1024},
    // {"MX25L2005",           0xC22012, 256, 16, 16, 256 * 1024},
    // {"MX25U4035",           0xC22533, 256, 16, 16, 512 * 1024},
    // {"MX25V4035",           0xC22553, 256, 16, 16, 512 * 1024},
    // {"MX25V4005",           0xC22013, 256, 16, 16, 512 * 1024},
    // {"MX25L4005",           0xC22013, 256, 16, 16, 512 * 1024},
    // {"MX25V8005",           0xC22014, 256, 16, 16, 1 * 1024 * 1024},
    // {"MX25L8005",           0xC22014, 256, 16, 16, 1 * 1024 * 1024},
    // {"MX25V8035",           0xC22554, 256, 16, 16, 1 * 1024 * 1024},
    // {"MX25U8035",           0xC22534, 256, 16, 16, 1 * 1024 * 1024},
    // {"MX25L8006E",          0xC22014, 256, 16, 16, 1 * 1024 * 1024},
    // {"MX25L1606E",          0xC22015, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L1605A",          0xC22015, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L1635D",          0xC22415, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L1633E",          0xC22415, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L1635E",          0xC22515, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L1636D",          0xC22415, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L1636E",          0xC22515, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L1608D",          0xC22015, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L1608E",          0xC22015, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25U1635E",          0xC22535, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L3225D",          0xC25E16, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25L3237D",          0xC25E16, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25L3236D",          0xC25E16, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25L3206E",          0xC22016, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25L3235D",          0xC25E16, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25U3235F",          0xC22536, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25U3235E",          0xC22536, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25L3208D",          0xC22016, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25L3208E",          0xC22016, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25L3205D",          0xC22016, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25L6455E",          0xC22617, 256, 16, 16, 8 * 1024 * 1024},
    // {"MX25L6465E",          0xC22017, 256, 16, 16, 8 * 1024 * 1024},
    // {"MX25L6408D",          0xC22017, 256, 16, 16, 8 * 1024 * 1024},
    // {"MX25L6408E",          0xC22017, 256, 16, 16, 8 * 1024 * 1024},
    // {"MX25L6406E",          0xC22017, 256, 16, 16, 8 * 1024 * 1024},
    // {"MX25L6445E",          0xC22017, 256, 16, 16, 8 * 1024 * 1024},
    // {"MX25L6405D",          0xC22017, 256, 16, 16, 8 * 1024 * 1024},
    // {"MX25L6436E",          0xC22017, 256, 16, 16, 8 * 1024 * 1024},
    // {"MX25L12805D",         0xC22018, 256, 16, 16, 16 * 1024 * 1024},
    // {"MX25L12835E",         0xC22018, 256, 16, 16, 16 * 1024 * 1024},
    // {"MX25L12835F",         0xC22018, 256, 16, 16, 16 * 1024 * 1024},
    // {"MX25U12835F",         0xC22538, 256, 16, 16, 16 * 1024 * 1024},
    // {"MX25L12845E",         0xC22018, 256, 16, 16, 16 * 1024 * 1024},
    // {"MX25L12836E",         0xC22018, 256, 16, 16, 16 * 1024 * 1024},
    // {"MX25L12865E",         0xC22018, 256, 16, 16, 16 * 1024 * 1024},
    // {"MX25L12855E",         0xC22618, 256, 16, 16, 16 * 1024 * 1024},
    // {"MX25L25635E",         0xC22019, 256, 16, 16, 32 * 1024 * 1024},
    // {"MX25L25635F",         0xC22019, 256, 16, 16, 32 * 1024 * 1024},
    // {"MX25L25639F",         0xC22019, 256, 16, 16, 32 * 1024 * 1024},
    // {"MX25L25735E",         0xC22019, 256, 16, 16, 32 * 1024 * 1024},
    // {"MX25L25735F",         0xC22019, 256, 16, 16, 32 * 1024 * 1024},
    // {"MX25U25635F",         0xC22539, 256, 16, 16, 32 * 1024 * 1024},
    // {"MX66U51235F",         0xC2253A, 256, 16, 16, 64 * 1024 * 1024},
    // {"MX66L51235F",         0xC2201A, 256, 16, 16, 64 * 1024 * 1024},
    // {"PM25LV512A",          0x7F9D7B, 256, 16, 16, 64 * 1024},
    // {"PM25LV010A",          0x7F9D7C, 256, 16, 16, 128 * 1024},
    // {"PM25LV020",           0x7F9D7D, 256, 16, 16, 256 * 1024},
    // {"PM25LV040",           0x7F9D7E, 256, 16, 16, 512 * 1024},
    // {"PM25LV080",           0x7F9D7F, 256, 16, 16, 64 * 1024},
    // {"S25FL001",            0x010210, 256, 16, 16, 128 * 1024},
    // {"S25FL002",            0x010211, 256, 16, 16, 256 * 1024},
    // {"S25FL004",            0x010212, 256, 16, 16, 512 * 1024},
    // {"S25FL008",            0x010213, 256, 16, 16, 1 * 1024 * 1024},
    // {"S25FL016",            0x010214, 256, 16, 16, 2 * 1024 * 1024},
    // {"S25FL032",            0x010215, 256, 16, 16, 4 * 1024 * 1024},
    // {"S25FL064",            0x010216, 256, 16, 16, 8 * 1024 * 1024},
    // {"S25FL128",            0x012017, 256, 16, 16, 16 * 1024 * 1024},
    // {"SST25WF512",          0xBF2501, 256, 16, 16, 64 * 1024},
    // {"SST25WF010",          0xBF2502, 256, 16, 16, 128 * 1024},
    // {"SST25WF020",          0xBF2503, 256, 16, 16, 256 * 1024},
    // {"SST25WF040",          0xBF2504, 256, 16, 16, 512 * 1024},
    // {"SST25VF040B",         0xBF258D, 256, 16, 16, 512 * 1024},
    // {"SST25VF080B",         0xBF258E, 256, 16, 16, 1 * 1024 * 1024},
    // {"SST25VF016B",         0xBF2541, 256, 16, 16, 2 * 1024 * 1024},
    // {"SST25VF032B",         0xBF254A, 256, 16, 16, 4 * 1024 * 1024},
    // {"SST25VF064B",         0xBF254B, 256, 16, 16, 8 * 1024 * 1024},
    // {"SST26VF016 ",         0xBF2601, 256, 16, 16, 2 * 1024 * 1024},
    // {"SST26VF032 ",         0xBF2602, 256, 16, 16, 4 * 1024 * 1024},
    // {"M25P05A",             0x202010, 256, 16, 16, 64 * 1024},
    // {"M25P10A",             0x202011, 256, 16, 16, 128 * 1024},
    // {"M25P20",              0x202012, 256, 16, 16, 256 * 1024},
    // {"M25P40",              0x202013, 256, 16, 16, 512 * 1024},
    // {"M25P80",              0x202014, 256, 16, 16, 1 * 1024 * 1024},
    // {"M25P16",              0x202015, 256, 16, 16, 2 * 1024 * 1024},
    // {"M25P32",              0x202016, 256, 16, 16, 4 * 1024 * 1024},
    // {"M25P64",              0x202017, 256, 16, 16, 8 * 1024 * 1024},
    // {"M25P128",             0x202018, 256, 16, 16, 16 * 1024 * 1024},
    // {"M25PX16",             0x207115, 256, 16, 16, 2 * 1024 * 1024},
    // {"M25PX32",             0x207116, 256, 16, 16, 4 * 1024 * 1024},
    // {"M25PX64",             0x207117, 256, 16, 16, 8 * 1024 * 1024},
    // {"M25PE10",             0x208011, 256, 16, 16, 128 * 1024},
    // {"M25PE20",             0x208012, 256, 16, 16, 256 * 1024},
    // {"M25PE40",             0x208013, 256, 16, 16, 512 * 1024},
    // {"M25PE80",             0x208014, 256, 16, 16, 1 * 1024 * 1024},
    // {"M25PE16",             0x208015, 256, 16, 16, 2 * 1024 * 1024}
//  ========================================================================
};

//-----------------------------------------------------------------------------
//����: ��״̬�Ĵ���
//����: regno��״̬�Ĵ������
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t SpiFlashReadSR(u8 regno, u8 *sta)
{
    u8 Command[1];
    bool_t ret = false;
    switch(regno)
    {
        case 1:
            Command[0] = CMD_SpiFlashReadStatusReg1;    //��״̬�Ĵ���1
            break;
        case 2:
            Command[0] = CMD_SpiFlashReadStatusReg2;    //��״̬�Ĵ���2
            break;
        case 3:
            Command[0] = CMD_SpiFlashReadStatusReg3;    //��״̬�Ĵ���3
            break;
        default:
            Command[0] = CMD_SpiFlashReadStatusReg1;
            break;
    }
#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
    //QSPI����ַλ0��4�ߴ����ݣ�8λ��ַ���޵�ַ��4�ߴ���ָ��޿����ڣ�1�ֽ�����
    ret = QSPI_Send_CMD(Command[0],0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES);
    if (ret)
    {
        ret = QSPI_Receive(sta, 1);
    }
#else
    //SPI
    struct SPI_DataFrame CommandFrame;
    CommandFrame.RecvBuf = sta;
    CommandFrame.RecvLen = 1;
    CommandFrame.RecvOff = 1;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 1;

    if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
    {
        if(CN_SPI_EXIT_NOERR ==
                SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
        {
            ret = true;
        }

        if (false == SPI_CsInactive(s_ptSpiPort))
        {
            error_printf("SpiFlash","spi cs inactive failed.\r\n");
        }
    }
#endif

    return ret;
}


//-----------------------------------------------------------------------------
//����: �ȴ�����
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t SpiFlashWaitBusy(u32 timeout)
{
    bool_t ret = true;
    u32 time = 0;
    u8 sta = 0;
    if(SpiFlashReadSR(1, &sta) == false)
    {
        return false;
    }
    while((sta & 0x01) == 0x01)   //�ȴ�����
    {
        if(SpiFlashReadSR(1, &sta) == false)
            return false;
        DJY_EventDelay(1000);
        time ++;
        if(time > timeout)
        {
            ret = false;
            error_printf("SpiFlash","Wait busy timeput.\r\n");
            break;
        }
    }
    return ret;
}

//-----------------------------------------------------------------------------
//����: д״̬�Ĵ���
//����: regno��״̬�Ĵ�����ţ�data: ��д�������
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t SpiFlashWriteSR(u8 regno, u8 data)
{
    bool_t ret = false;
    u8 Command[2];
    switch(regno)
    {
        case 1:
            Command[0] = CMD_SpiFlashWriteStatusReg1;    //д״̬�Ĵ���1
            break;
        case 2:
            Command[0] = CMD_SpiFlashWriteStatusReg2;    //д״̬�Ĵ���2
            break;
        case 3:
            Command[0] = CMD_SpiFlashWriteStatusReg3;    //д״̬�Ĵ���3
            break;
        default:
            Command[0] = CMD_SpiFlashWriteStatusReg1;
            break;
    }

    if(SpiFlashWaitBusy(5000) == false)
    {
        return false;
    }

#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
    //QSPI����ַλ0��4�ߴ����ݣ�8λ��ַ���޵�ַ��4�ߴ���ָ��޿����ڣ�1�ֽ�����
    ret = QSPI_Send_CMD(Command[0],0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES);
    if (ret)
    {
        ret = QSPI_Transmit(&data, 1);
    }
#else
    //SPI
    struct SPI_DataFrame CommandFrame;
    Command[1] = data;
    CommandFrame.RecvBuf = NULL;
    CommandFrame.RecvLen = 0;
    CommandFrame.RecvOff = 0;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 2;

    if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
    {
        if(CN_SPI_EXIT_NOERR ==
                SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
        {
            ret = true;
        }

        if (false == SPI_CsInactive(s_ptSpiPort))
        {
            error_printf("SpiFlash","spi cs inactive failed.\r\n");
        }
    }
#endif
    return ret;
}

//-----------------------------------------------------------------------------
//����: дʹ�ܵȴ�
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t SpiFlashWriteEnableWait(void)
{
    u8 sta = 0;
    u32 wait = 40;
    if(SpiFlashReadSR(1, &sta))
    {
        while((sta & 2) != 2)
        {
            wait --;
            if(wait == 0)
                return false;
            sta = 0;
            SpiFlashReadSR(1, &sta);
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
//����: дʹ��
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t SpiFlashWriteEnable(void)
{
    
    if(SpiFlashWaitBusy(5000) == false)
    {
        return false;
    }

#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
    //QSPI����ַλ0�������ݣ�8λ��ַ���޵�ַ��4�ߴ���ָ��޿����ڣ�0�ֽ�����
    if(QSPI_Send_CMD(CMD_SpiFlashWriteEnable,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE) == false)
    {
        return false;
    }
#else
    //SPI��
    struct SPI_DataFrame CommandFrame;
    bool_t Ret = false;
    u8 Command[1] = {CMD_SpiFlashWriteEnable};

    CommandFrame.RecvBuf = NULL;
    CommandFrame.RecvLen = 0;
    CommandFrame.RecvOff = 0;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 1;

    if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
    {
        if(CN_SPI_EXIT_NOERR ==
                SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
        {
            Ret = true;
        }

        if (false == SPI_CsInactive(s_ptSpiPort))
        {
            error_printf("SpiFlash","spi cs inactive failed.\r\n");
        }
    }

    if (false == Ret)
    {
        return false;
    }
#endif
    return SpiFlashWriteEnableWait();
}

//-----------------------------------------------------------------------------
//����: дʧ��
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t SpiFlashWriteDisable(void)
{
    bool_t ret = false;

    if(SpiFlashWaitBusy(5000) == false)
    {
        return false;
    }

#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
    //QSPI����ַλ0�������ݣ�8λ��ַ���޵�ַ��4�ߴ���ָ��޿����ڣ�0�ֽ�����
    ret = QSPI_Send_CMD(CMD_SpiFlashWriteDisable,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
#else
    //SPI
    struct SPI_DataFrame CommandFrame;
    u8 Command[1] = {CMD_SpiFlashWriteDisable};

    CommandFrame.RecvBuf = NULL;
    CommandFrame.RecvLen = 0;
    CommandFrame.RecvOff = 0;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 1;

    if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
    {
        if(CN_SPI_EXIT_NOERR ==
                SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
        {
            ret = true;
        }

        if (false == SPI_CsInactive(s_ptSpiPort))
        {
            error_printf("SpiFlash","spi cs inactive failed.\r\n");
        }
    }
#endif
    return ret;
}

//-----------------------------------------------------------------------------
//����: ��ID
//����: ��
//����: оƬID
//��ע:
//-----------------------------------------------------------------------------
u32 SpiFlashReadID(void)
{
    u8 temp[3];
    u32 deviceid = 0;
    bool_t ret = false;

    if(SpiFlashWaitBusy(5000) == false)
    {
        return false;
    }

#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
    //QSPI����ַλ0��4�ߴ������ݣ�24λ��ַ��4�ߴ����ַ��4�ߴ���ָ��޿����ڣ�2�ֽ�����
    ret = QSPI_Send_CMD(CMD_SpiFlashJedecDeviceID,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES);

    if(ret)
    {
        QSPI_Receive(temp,3);
        deviceid=(temp[0] << 16) | (temp[1] << 8) | temp[2];
    }
    else
    {
        deviceid = 0;
    }
#else
    //SPI
    struct SPI_DataFrame CommandFrame;
    u8 Command = CMD_SpiFlashJedecDeviceID;

    CommandFrame.RecvBuf = temp;
    CommandFrame.RecvLen = 3;
    CommandFrame.RecvOff = 1;
    CommandFrame.SendBuf = &Command;
    CommandFrame.SendLen = 1;

    if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
    {
        if(CN_SPI_EXIT_NOERR ==
                SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
        {
            deviceid = (temp[0] << 16) | (temp[1] << 8) | temp[2];
        }

        if (false == SPI_CsInactive(s_ptSpiPort))
        {
            error_printf("SpiFlash","spi cs inactive failed.\r\n");
        }
    }
#endif
    return deviceid;
}
//-----------------------------------------------------------------------------
//����: оƬ����
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t SpiFlashEraseChip(void)
{
    bool_t res = false;
    if(Lock_MutexPend(SpiFlashLock,CN_TIMEOUT_FOREVER))
    {
        if(SpiFlashWriteEnable())
        {
            if(SpiFlashWaitBusy(5000))
            {
#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
                //QSPI����ַΪ0�������ݣ�8λ��ַ���޵�ַ��4�ߴ���ָ��޿�����
                res = QSPI_Send_CMD(CMD_SpiFlashChipErase,0,0,QSPI_INSTRUCTION_4_LINES,
                                    QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
#else
                // SPI
                struct SPI_DataFrame CommandFrame;
                u8 Command[1] = {CMD_SpiFlashChipErase};

                CommandFrame.RecvBuf = NULL;
                CommandFrame.RecvLen = 0;
                CommandFrame.RecvOff = 0;
                CommandFrame.SendBuf = Command;
                CommandFrame.SendLen = 1;

                if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
                {
                    if(CN_SPI_EXIT_NOERR ==
                            SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
                    {
                        res = true;
                    }

                    if (false == SPI_CsInactive(s_ptSpiPort))
                    {
                        error_printf("SpiFlash","spi cs inactive failed.\r\n");
                    }
                }
#endif
                if ((res) && (SpiFlashWaitBusy(500000)))
                {
                    res = true;
                }
            }
        }
        Lock_MutexPost(SpiFlashLock);
    }
    return res;
}

//-----------------------------------------------------------------------------
//����: ��������
//����: addr��������ַ
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t SpiFlashEraseSector(u32 addr)
{
    bool_t res = false;
    addr *= 4096;
    if(Lock_MutexPend(SpiFlashLock,CN_TIMEOUT_FOREVER))
    {
        if(SpiFlashWriteEnable())
        {
#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
            //QSPI����ַΪaddr�������ݣ�32λ��ַ��4�ߴ����ַ��4�ߴ���ָ��޿�����
            res = QSPI_Send_CMD(CMD_SpiFlashSectorErase,addr,0,QSPI_INSTRUCTION_4_LINES,
                                    QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_NONE);
#else
            // SPI
            struct SPI_DataFrame CommandFrame;
            u8 Command[5];

            if (Enable4B_AddrOpt)
            {
                Command[0] = CMD_SpiFlashSectorErase_4B_Addr;
                Command[1] = ((addr >> 24) & 0xFF);
                Command[2] = ((addr >> 16) & 0xFF);
                Command[3] = ((addr >> 8) & 0xFF);
                Command[4] = (addr & 0xFF);
                CommandFrame.SendLen = 5;
            }
            else
            {
                Command[0] = CMD_SpiFlashSectorErase;
                Command[1] = ((addr >> 16) & 0xFF);
                Command[2] = ((addr >> 8) & 0xFF);
                Command[3] = (addr & 0xFF);
                CommandFrame.SendLen = 4;
            }

            CommandFrame.RecvBuf = NULL;
            CommandFrame.RecvLen = 0;
            CommandFrame.RecvOff = 0;
            CommandFrame.SendBuf = Command;

            if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
            {
                if(CN_SPI_EXIT_NOERR ==
                        SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
                {
                    res = true;
                }

                if (false == SPI_CsInactive(s_ptSpiPort))
                {
                    error_printf("SpiFlash","spi cs inactive failed.\r\n");
                }
            }
#endif
        }
        Lock_MutexPost(SpiFlashLock);
    }
    return res;
}

//-----------------------------------------------------------------------------
//����: ��������
//����: addr��������ַ
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t SpiFlashEraseBlock(u32 addr)
{
    bool_t res = false;
    addr *= 65536;
    if(Lock_MutexPend(SpiFlashLock,CN_TIMEOUT_FOREVER))
    {
        if(SpiFlashWriteEnable())
        {
#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
            //QSPI����ַΪaddr�������ݣ�32λ��ַ��4�ߴ����ַ��4�ߴ���ָ��޿�����
            res = QSPI_Send_CMD(CMD_SpiFlashBlockErase,addr,0,QSPI_INSTRUCTION_4_LINES,
                                QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_NONE);
#else
            // SPI
            struct SPI_DataFrame CommandFrame;
            u8 Command[5];

            if (Enable4B_AddrOpt)
            {
                Command[0] = CMD_SpiFlashBlockErase_4B_Addr;
                Command[1] = ((addr >> 24) & 0xFF);
                Command[2] = ((addr >> 16) & 0xFF);
                Command[3] = ((addr >> 8) & 0xFF);
                Command[4] = (addr & 0xFF);
                CommandFrame.SendLen = 5;
            }
            else
            {
                Command[0] = CMD_SpiFlashBlockErase;
                Command[1] = ((addr >> 16) & 0xFF);
                Command[2] = ((addr >> 8) & 0xFF);
                Command[3] = (addr & 0xFF);
                CommandFrame.SendLen = 4;
            }

            CommandFrame.RecvBuf = NULL;
            CommandFrame.RecvLen = 0;
            CommandFrame.RecvOff = 0;
            CommandFrame.SendBuf = Command;

            if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
            {
                if(CN_SPI_EXIT_NOERR ==
                        SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
                {
                    res = true;
                }

                if (false == SPI_CsInactive(s_ptSpiPort))
                {
                    error_printf("SpiFlash","spi cs inactive failed.\r\n");
                }
            }
#endif
        }
        Lock_MutexPost(SpiFlashLock);
    }
    return res;
}

//-----------------------------------------------------------------------------
//����: ��flash
//����: buf�����ݻ�������addr�����ĵ�ַ��len����ȡ���ֽ���
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t SpiFlashRead(u8* buf,u32 addr,u32 len)
{
    bool_t ret = false;
    if(Lock_MutexPend(SpiFlashLock,CN_TIMEOUT_FOREVER))
    {
        if(SpiFlashWaitBusy(5000) == false)
        {
            ret = false;
        }
#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
        //QSPI�����ٶ����ݣ���ַΪaddr��4�ߴ������ݣ�32λ��ַ��4�ߴ����ַ��4�ߴ���ָ�2�������ڣ�
        if(QSPI_Send_CMD(CMD_SpiFlashFastReadData,addr,2,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_4_LINES))
        {
            if(QSPI_Receive(buf, len))
            {
                ret = true;
            }
            else
            {
                ret = false;
            }
        }
#else
        struct SPI_DataFrame CommandFrame;
        u8 Command[5];

        if (Enable4B_AddrOpt)
        {
            Command[0] = CMD_SpiFlashReadData_4B_Addr;
            Command[1] = ((addr >> 24) & 0xFF);
            Command[2] = ((addr >> 16) & 0xFF);
            Command[3] = ((addr >> 8) & 0xFF);
            Command[4] = (addr & 0xFF);
            CommandFrame.RecvOff = 5;
            CommandFrame.SendLen = 5;
        }
        else
        {
            Command[0] = CMD_SpiFlashReadData;
            Command[1] = ((addr >> 16) & 0xFF);
            Command[2] = ((addr >> 8) & 0xFF);
            Command[3] = (addr & 0xFF);
            CommandFrame.RecvOff = 4;
            CommandFrame.SendLen = 4;
        }

        CommandFrame.RecvBuf = buf;
        CommandFrame.RecvLen = len;
        CommandFrame.SendBuf = Command;

        if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
        {
            if(CN_SPI_EXIT_NOERR ==
                    SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
            {
                ret = true;
            }

            if (false == SPI_CsInactive(s_ptSpiPort))
            {
                error_printf("SpiFlash","spi cs inactive failed.\r\n");
            }
        }

#endif
        Lock_MutexPost(SpiFlashLock);
    }
    return ret;
}

//-----------------------------------------------------------------------------
//����: дһҳflash
//����: buf�����ݻ�������addr��д�ĵ�ַ��len��д���ֽ���
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t SpiFlashWritePage(u8* buf,u32 addr,u32 len)
{
    bool_t res = false;
    if(Lock_MutexPend(SpiFlashLock,CN_TIMEOUT_FOREVER))
    {
        if(SpiFlashWriteEnable())
        {
#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
            //QSPI��ҳд����ַΪaddr��4�ߴ������ݣ�32λ��ַ��4�ߴ����ַ��4�ߴ���ָ�0�������ڣ�
            if(QSPI_Send_CMD(CMD_SpiFlashPageProgram,addr,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_4_LINES))
            {
                res = QSPI_Transmit(buf, len);
            }
#else
            struct SPI_DataFrame CommandFrame;
            u8 Command[5];

            if (Enable4B_AddrOpt)
            {
                Command[0] = CMD_SpiFlashPageProgram_4B_Addr;
                Command[1] = ((addr >> 24) & 0xFF);
                Command[2] = ((addr >> 16) & 0xFF);
                Command[3] = ((addr >> 8) & 0xFF);
                Command[4] = (addr & 0xFF);
                CommandFrame.SendLen = 5;
            }
            else
            {
                Command[0] = CMD_SpiFlashPageProgram;
                Command[1] = ((addr >> 16) & 0xFF);
                Command[2] = ((addr >> 8) & 0xFF);
                Command[3] = (addr & 0xFF);
                CommandFrame.SendLen = 4;
            }

            CommandFrame.RecvBuf = NULL;
            CommandFrame.RecvLen = 0;
            CommandFrame.RecvOff = 0;
            CommandFrame.SendBuf = Command;

            if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
            {
                if(CN_SPI_EXIT_NOERR == SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
                {
                    CommandFrame.RecvBuf = NULL;
                    CommandFrame.RecvLen = 0;
                    CommandFrame.RecvOff = 0;
                    CommandFrame.SendBuf = buf;
                    CommandFrame.SendLen = len;
                    if(CN_SPI_EXIT_NOERR == SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
                    {
                        res = true;
                    }
                }
                if (false == SPI_CsInactive(s_ptSpiPort))
                {
                    error_printf("SpiFlash","spi cs inactive failed.\r\n");
                }
            }
#endif
        }
        Lock_MutexPost(SpiFlashLock);
    }
    return res;
}

//-----------------------------------------------------------------------------
//����: �����������ܵ�д��д֮ǰ��Ҫȷ����д�ĵ�ַ��Χ�ڵ�����ȫΪ0xff�������д��ʧ��
//����: buf�����ݻ�������addr��д�ĵ�ַ��len����ȡ���ֽ���
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t SpiFlashWriteNoErase(u8* buf,u32 addr,u32 len)
{
    u32 remain = 0;
    bool_t ret = false;

    if(Lock_MutexPend(SpiFlashLock,CN_TIMEOUT_FOREVER))
    {
        remain = FlashPageSize - addr % FlashPageSize;  //��ҳ��ʣ����ֽ���
        if(len <= remain)
            remain = len;
        while(1)
        {
            if(SpiFlashWritePage(buf, addr, remain))
            {
                if(remain == len)
                {
                    ret = true;
                    break;
                }
                else
                {
                    buf += remain;
                    addr += remain;
                    len -= remain;
                    if(len > FlashPageSize)
                        remain = FlashPageSize;
                    else
                        remain = len;
                }
            }
            else
                break;

        }
        Lock_MutexPost(SpiFlashLock);
    }
    return ret;
}

//-----------------------------------------------------------------------------
//����: дflash
//����: buf�����ݻ�������addr��д�ĵ�ַ��len����ȡ���ֽ���
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
u8 SPI_FLASH_BUFFER[4096];
bool_t SpiFlashWrite(u8* buf,u32 addr,u32 len)
{
    u32 sec;
    u32 sec_off;
    u32 sec_remain;
    u32 i;
    bool_t ret = true;

    if(Lock_MutexPend(SpiFlashLock,CN_TIMEOUT_FOREVER))
    {
        sec = addr / 4096; //������ַ
        sec_off = addr % 4096;//����ƫ��
        sec_remain = 4096- sec_off; //����ʣ���С

        if(len <= sec_remain)
            sec_remain = len;
        while(1)
        {
            if(SpiFlashRead(SPI_FLASH_BUFFER, sec*4096, 4096))
            {
                for(i=0; i < sec_remain; i++)//
                {
                    if ((SPI_FLASH_BUFFER[sec_off + i] != 0XFF) && (SPI_FLASH_BUFFER[sec_off + i] != buf[i]))
                        break;
                }
                if(i < sec_remain)    //��Ҫ����
                {
                    if(SpiFlashEraseSector(sec))
                    {
                        for(i=0; i<sec_remain; i++)
                        {
                            SPI_FLASH_BUFFER[i + sec_off] = buf[i];
                        }
                        if(SpiFlashWriteNoErase(SPI_FLASH_BUFFER, sec*4096, 4096) == false)
                        {
                            ret = false;
                            break;
                        }
                    }
                    else
                    {
                        ret = false;
                        break;
                    }

                }
                else
                {
                    if(SpiFlashWriteNoErase(buf, addr, sec_remain) == false)
                    {
                        ret = false;
                        break;
                    }
                }
                if(len == sec_remain)
                    break;  //д����
                else
                {
                    sec++;
                    sec_off=0;

                    buf += sec_remain;
                    addr += sec_remain;
                    len -= sec_remain;
                    if(len > 4096)
                        sec_remain=4096;
                    else
                        sec_remain=len;
                }
            }
            else
            {
                ret = false;
                break;
            }
        }
        Lock_MutexPost(SpiFlashLock);
    }
    return ret;
}

#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
//-----------------------------------------------------------------------------
//����: flash Qspiʹ��
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t SpiFlashQspiEnable(void)
{
    u8 sta2;
    bool_t ret = true;

    if(SpiFlashReadSR(2, &sta2) == false)       //��״̬�Ĵ���2��ֵ
        return false;
    if((sta2 & 0X02)==0)           //QEλδʹ��
    {
        SpiFlashWriteEnable();      //дʹ��
        sta2 |= 1<<1;              //ʹ��QEλ
        if(SpiFlashWriteSR(2, sta2) == false) //д״̬�Ĵ���2
            ret = false;

        //дʹ��QSPIָ���ַΪ0�������ݣ�8λ��ַ���޵�ַ�����ߴ���ָ��޿����ڣ�0���ֽ�����
//        QSPI_Send_CMD(CMD_SpiFlashExitQPIMode,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
        if(QSPI_Send_CMD(CMD_SpiFlashEnterQPIMode,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE))
        {
            if(SpiFlashReadSR(2, &sta2))       //��״̬�Ĵ���2��ֵ
            {
                if((sta2 & 0X02)==0)           //QEλδʹ��
                    ret = false;
            }
            else
                ret = false;
        }
        else
            ret = false;
    }
    else
    {
        ret = QSPI_Send_CMD(CMD_SpiFlashEnterQPIMode,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
    }
    return ret;
}

//-----------------------------------------------------------------------------
//����: SpiFlashQspiʧ��
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t SpiFlashQspiDisable(void)
{
    //дʧ��QSPIָ���ַΪ0�������ݣ�8λ��ַ���޵�ַ��4�ߴ���ָ��޿����ڣ�0���ֽ�����
    if(QSPI_Send_CMD(CMD_SpiFlashExitQPIMode,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE))
    {
        return true;
    }
    else
        return false;
}

#endif
//-----------------------------------------------------------------------------
//����: SpiFlash��ʼ��
//����: ��
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
bool_t SpiFlashInit(void)
{
    u32 chip_id;
    u32 i = 0;
    bool_t ret = false;

#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
    u8 temp;
    if (false == SpiFlashQspiEnable())
    {
        return false;
    }
#else
    s_ptSpiPort = SPI_DevAdd(CFG_SPI_FLASH_SPI_NAME,SpiFlashName,0,8,SPI_MODE_0,SPI_SHIFT_MSB,1000*1000,false);
    if(s_ptSpiPort)
    {
        SPI_BusCtrl(s_ptSpiPort,CN_SPI_SET_POLL,0,0);
        SPI_BusCtrl(s_ptSpiPort,CN_SPI_SET_CLK, 30 * 1000 * 1000, 0);
    }
    else
    {
        error_printf("SpiFlash","Add spi dev failed.\n\r");
        return false;
    }
#endif

    chip_id = SpiFlashReadID();

    for (i = 0; i < sizeof(FlashInfo) / sizeof(struct SPI_FlashInfo); i++)
    {
        if (chip_id == FlashInfo[i].ChipID)
        {
            if (FlashInfo[i].Capacity > (16 * 1024 * 1024))
            {
                Enable4B_AddrOpt = true;    //����16M�Ļ���3�ֽڵ�ַ�Ͳ��ܷ����ˣ�Ҫ��4�ֽڵ�ַ�������
            }
            FlashPageSize = FlashInfo[i].PageSize;
            FlashPageInSector = FlashInfo[i].PageNumInSector;
            FlashSectorInBolck = FlashInfo[i].SectorNumInBlock;
            FlashSectorNum = FlashInfo[i].Capacity / (FlashPageSize * FlashPageInSector);
            ret = true;
            break;
        }
    }
    
 #if CFG_SPI_FLASH_QSPI_ENABLE == TRUE       
    if(SpiFlashReadSR(3, &temp))      //��״̬�Ĵ���3
    {
        if((temp & 0X01) == 0)          //�жϵ�ǰ�Ƿ�Ϊ4��ַģʽ��0��3��ַģʽ��1��4��ַģʽ
        {
            if(SpiFlashWriteEnable())  //дʹ��
            {
                //дʹ��4�ֽڵ�ַָ���ַΪ0�������ݣ�8λ��ַ���޵�ַ��4�ߴ���ָ��޿����ڣ�0���ֽ�����
                if(QSPI_Send_CMD(SpiFlashEnable4ByteAddr,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE) == false)
                    return ret;
            }
            else
                return ret;
        }
        if(SpiFlashWriteEnable())
        {
            //д���ö�����ָ���ַΪ0��4�ߴ������ݣ�8λ��ַ���޵�ַ��4�ߴ���ָ��޿����ڣ�0���ֽ�����
            if(QSPI_Send_CMD(SpiFlashSetReadParam,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES))
            {
                temp = 0<<4;                  //����P4&P5=00,2��dummy clocks,44M
                if(QSPI_Transmit(&temp,1))
                    ret = true;
            }
        }
    }
#endif
    return ret;
}


// ============================================================================
// ���ܣ�QFLASH ����
// ������ucmd -- ���
//      ���� -- ���������
// ���أ�0 -- �ɹ���-1 -- ʧ��
// ��ע��
// ============================================================================
s32 __SpiFlashReq(enum ucmd cmd, ptu32_t args, ...)
{
    s32 res = 0;

    switch(cmd)
    {
        case whichblock://��ҳ�ڵڼ�����
        {
            va_list list;
            u32 *block;
            s64 *unit;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);
            *block = *unit / SpiFlashdescription->PagesPerBlock;
            break;
        }

        case totalblocks:
        {
            *((u32*)args) =  SpiFlashdescription->BlockNum;
            break;
        }
        case totalSectors:
        {
            *((u32*)args) =  SpiFlashdescription->SectorNum;
            break;
        }

        case sectorsunits:
        {
            // args = &units
            *((u32*)args) = SpiFlashdescription->PagesPerSector;
            break;
        }

        case blockunits:
        {
            // args = &units
            *((u32*)args) = SpiFlashdescription->PagesPerBlock;
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = SpiFlashdescription->BytesPerPage;
            break;
        }

        case format:
        {
            va_list list;
            u32 start, end;
            struct uesz *sz;

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            sz = (struct uesz*)va_arg(list, u32);
            va_end(list);

            if(-1 == (s32)end)
                end = SpiFlashdescription->SectorNum;
// else if (start)
//     end += start;

            do
            {
                if(SpiFlashEraseBlock(--end) == false)
                {
                    res = -1;
                    break;
                }
            }
            while(end!=start);
            break;
        }
        default: res = -1;
        break;
    }

    return (res);
}

//-----------------------------------------------------------------------------
//����: Norģ�鰲װ
//����:
//����: true -- �ɹ�; false -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 ModuleInstall_SpiFlash(void)
{
    SpiFlashLock = Lock_MutexCreate("spi flash lock");
    if(!SpiFlashLock)
    {
        error_printf("SpiFlash","MODULE INSTALL : error : cannot create spi flash lock.");
        return false;
    }

    if(SpiFlashInit() == false)
    {
        error_printf("SpiFlash","Install fail.\r\n");
        return false;
    }

    if(!SpiFlashdescription)
    {
        SpiFlashdescription = malloc(sizeof(struct NorDescr));
        if(!SpiFlashdescription)
        {
            error_printf("SpiFlash","erro : device : memory out.\r\n");
            return false;
        }
        memset(SpiFlashdescription, 0x0, (sizeof(struct NorDescr)));
        SpiFlashdescription->PortType = NOR_SPI;
        SpiFlashdescription->Port = NULL;
        SpiFlashdescription->BytesPerPage = FlashPageSize;
        SpiFlashdescription->SectorNum = FlashSectorNum;
        SpiFlashdescription->BlockNum = FlashSectorNum / FlashSectorInBolck;
        SpiFlashdescription->BlocksPerSector = 0;
        SpiFlashdescription->PagesPerSector = FlashPageInSector;
        SpiFlashdescription->SectorsPerBlock = FlashSectorInBolck;
        SpiFlashdescription->PagesPerBlock = FlashPageInSector * FlashSectorInBolck;
        SpiFlashdescription->ReservedBlks = 0;
    }

    if(CFG_SPI_FLASH_PART_ERASE)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        if(-1 == __SpiFlashReq(format, 0 , -1, &sz))
        {
            warning_printf("SpiFlash"," Format failure.\r\n");
        }
    }

    SpiFlashUm = malloc(sizeof(struct umedia));
    if(!SpiFlashUm)
    {
        return (-1);
    }
    SpiFlashUm->mreq = __SpiFlashReq;
    SpiFlashUm->type = nor;
    SpiFlashUm->ubuf = NULL;

    if(!Device_Create((const char*)SpiFlashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)SpiFlashUm)))
    {
        printf("\r\n: erro : device : %s addition failed.", SpiFlashName);
        free(SpiFlashUm);
        return (-1);
    }

    SpiFlashInitFlag = true;

    return true;

}

// =============================================================================
// ���ܣ��ж�flash�Ƿ�װ
// ������  ��
// ���أ��ѳɹ���װ��true����δ�ɹ���װ��false����
// ��ע��
// =============================================================================
bool_t SpiFlashIsInstall(void)
{
    return SpiFlashInitFlag;
}

// ============================================================================
// ���ܣ���ʼ��Ƭ��flash
// ������fs -- ��Ҫ���ص��ļ�ϵͳ��mediadrv -- ý��������
//       bstart -- ��ʼ�飬bend -- �����飨�������ÿ飬ֻ���ÿ����һ�飩
// ���أ�0 -- �ɹ��� -1 -- ʧ��
// ��ע��
// ============================================================================
s32 __SpiFlashFsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv)
{
     u32 units, total = 0,endblock = bend;
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res;
    targetobj = OBJ_MatchPath(fs, &notfind);
    if(notfind)
    {
        error_printf("embed"," not found need to install file system.\r\n");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj);
    super->MediaInfo = SpiFlashUm;
    super->MediaDrv = mediadrv;

     if(-1 == (s32)endblock)
     {
         endblock = bend = SpiFlashdescription->BlockNum; // �����
     }

     do
     {
         if(__SpiFlashReq(blockunits, (ptu32_t)&units, --endblock))
         {
             return (-1);
         }

         total += units;
     }
     while(endblock!=bstart);

    super->AreaSize = SpiFlashdescription->BytesPerPage * total;
    // endblock = 0;
    // total = 0;
    // while(endblock<bstart)
    // {
    //     if(__embed_req(blockunits, (ptu32_t)&units, endblock++))
    //     {
    //         return (-1);
    //     }
    //     total += units;
    // }

    super->MediaStart = bstart * SpiFlashdescription->PagesPerBlock; // ��ʼunit��

    res = strlen(SpiFlashName) + strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,SpiFlashName);   //��ȡ���豸��ȫ·��
    File_BeMedia(FullPath,fs); //�����豸�����ļ�ϵͳ
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);

}

//-----------------------------------------------------------------------------
//����: ��������
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
void SpiFlashLocalTest(void)
{
    u32 i, j, k;
    u16 temp;
    u8 WriteBuf[FlashPageSize];
    u8 ReadBuf[FlashPageSize];
    temp = SpiFlashReadID();
    debug_printf("SpiFlash"," chip id is 0x%4x.\r\n", temp);

    for(i=0; i<SpiFlashdescription->BytesPerPage; i++)
    {
        WriteBuf[i] = i;
    }
    // ���д�����
    if(false == SpiFlashEraseChip())
    {
        error_printf("SpiFlash"," erase chip failed.\r\n");
    }
    else
    {
        for(k=0; k<SpiFlashdescription->BlockNum; k++)
        {
            for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != 0xFF)
                        {
                            error_printf("SpiFlash"," page erase %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                }
            }


            for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
            {
                if (false == SpiFlashWritePage(WriteBuf, 
                        (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(WriteBuf)))
                {
                    error_printf("SpiFlash"," write block %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                }
            }

            for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != WriteBuf[j])
                        {
                            error_printf("SpiFlash"," page %d data error.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                }
            }
            if (false == SpiFlashEraseBlock(k))
            {
                error_printf("SpiFlash"," erase block %d failed.\r\n", k);
            }

            for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != 0xFF)
                        {
                            error_printf("SpiFlash"," page erase %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                }
            }
        }
    }

    // ��д��flash
    for(k=0; k<SpiFlashdescription->BlockNum; k++)
    {
        for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != 0xFF)
                    {
                        error_printf("SpiFlash"," page erase %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
            }
        }


        for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
        {
            if (false == SpiFlashWritePage(WriteBuf, 
                    (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(WriteBuf)))
            {
                error_printf("SpiFlash"," write block %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
            }
        }

        for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != WriteBuf[j])
                    {
                        error_printf("SpiFlash"," page %d data error.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
            }
        }
    }

    // ����Ƭ����
    if(false == SpiFlashEraseChip())
    {
        error_printf("SpiFlash"," erase chip failed.\r\n");
    }
    else
    {
        for(k=0; k<SpiFlashdescription->BlockNum; k++)
        {
            for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != 0xFF)
                        {
                            error_printf("SpiFlash"," page erase %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                }
            }

        }
    }

    // ����������
    for(k=0; k<SpiFlashdescription->SectorNum; k++)
    {
        for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerSector)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != 0xFF)
                    {
                        error_printf("SpiFlash"," page erase %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
            }
        }


        for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
        {
            if (false == SpiFlashWritePage(WriteBuf, 
                    (i+(k*(SpiFlashdescription->PagesPerSector)))*SpiFlashdescription->BytesPerPage, sizeof(WriteBuf)))
            {
                error_printf("SpiFlash"," write sector %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
            }
        }

        for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerSector)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != WriteBuf[j])
                    {
                        error_printf("SpiFlash"," page %d data error.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
            }
        }
        if (false == SpiFlashEraseSector(k))
        {
            error_printf("SpiFlash"," erase block %d failed.\r\n", k);
        }

        for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerSector)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != 0xFF)
                    {
                        error_printf("SpiFlash"," page erase %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
            }
        }
    }

    // ����оƬ��д��2����
    if(false == SpiFlashEraseChip())
    {
        error_printf("SpiFlash"," erase chip failed.\r\n");
    }
    else
    {
        for(k=0; k<2; k++)
        {
            for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != 0xFF)
                        {
                            error_printf("SpiFlash"," page erase %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                }
            }


            for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
            {
                if (false == SpiFlashWritePage(WriteBuf, 
                        (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(WriteBuf)))
                {
                    error_printf("SpiFlash"," write block %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                }
            }

            for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != WriteBuf[j])
                        {
                            error_printf("SpiFlash"," page %d data error.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                }
            }
        }
        // ������һ����
        if (false == SpiFlashEraseBlock(0))
        {
            error_printf("SpiFlash"," erase block %d failed.\r\n", 0);
        }
        // ��һ������ȫff
        for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, i*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != 0xFF)
                    {
                        error_printf("SpiFlash"," page erase %d failed.\r\n", i);
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", i);
            }
        }
        // �ڶ����黹������
        for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, (i+SpiFlashdescription->PagesPerBlock)*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != WriteBuf[j])
                    {
                        error_printf("SpiFlash"," page %d data error.\r\n", (i+SpiFlashdescription->PagesPerBlock));
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", (i+SpiFlashdescription->PagesPerBlock));
            }
        }
    }

    // ��д2��������һ������
    for(k=0; k<2; k++)
    {
        for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerSector)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != 0xFF)
                    {
                        error_printf("SpiFlash"," page erase %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
            }
        }


        for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
        {
            if (false == SpiFlashWritePage(WriteBuf, 
                    (i+(k*(SpiFlashdescription->PagesPerSector)))*SpiFlashdescription->BytesPerPage, sizeof(WriteBuf)))
            {
                error_printf("SpiFlash"," write block %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
            }
        }

        for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerSector)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != WriteBuf[j])
                    {
                        error_printf("SpiFlash"," page %d data error.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
            }
        }
    }
    // ������һ����
    if (false == SpiFlashEraseSector(0))
    {
        error_printf("SpiFlash"," erase block %d failed.\r\n", 0);
    }
    // ��һ������ȫff
    for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
    {
        memset(ReadBuf, 0, sizeof(ReadBuf));
        if (SpiFlashRead(ReadBuf, i*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
        {
            for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
            {
                if(ReadBuf[j] != 0xFF)
                {
                    error_printf("SpiFlash"," page erase %d failed.\r\n", i);
                    break;
                }
            }
        }
        else
        {
            error_printf("SpiFlash"," read page %d failed.\r\n", i);
        }
    }
    // �ڶ���������������
    for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
    {
        memset(ReadBuf, 0, sizeof(ReadBuf));
        if (SpiFlashRead(ReadBuf, (i+SpiFlashdescription->PagesPerSector)*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
        {
            for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
            {
                if(ReadBuf[j] != WriteBuf[j])
                {
                    error_printf("SpiFlash"," page %d data error.\r\n", (i+SpiFlashdescription->PagesPerSector));
                    break;
                }
            }
        }
        else
        {
            error_printf("SpiFlash"," read page %d failed.\r\n", (i+SpiFlashdescription->PagesPerSector));
        }
    }

}




