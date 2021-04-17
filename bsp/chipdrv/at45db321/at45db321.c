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
//����ģ�飺SPI FLASHģ��
//���ߣ�liq
//�汾��V0.1.0
//�ļ�������DJYOS��SPI FLASH AT45DB321D�ײ�����
//����˵����
//�޶���ʷ��
//1. ����: 2014-4-15
//   ���ߣ�
//   �°汾�ţ�V0.1.0
//   �޸�˵����ԭʼ�汾
//------------------------------------------------------

#include "stdint.h"
#include "string.h"
#include "cpu_peri.h"
#include "at45db321.h"
#include "spibus.h"
#include "board-config.h"
#include <device/djy_flash.h>
#include "os.h"
#include "systime.h"
#include <math.h>
#include <stdlib.h>
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <device.h>
#include <device/unit_media.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_at45db321(void);
//    ModuleInstall_at45db321();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"nor flash at45db321"//spi�ӿڵ�norflash
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","spi bus","heap","cpu onchip spi"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"yaf2 file system","easy file system"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_NOR_FLASH_AT45DB321 == false )
//#warning  " nor_flash_at45db321  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_NOR_FLASH_AT45DB321    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,512,528,
#define CFG_AT45_PAGE_SIZE                  512       //����AT45��ҳ��С��Ĭ��Ϊ512
//%$#@string,1,10,
#define CFG_AT45_BUSNAME                   "SPI4"     //"SPI��������",AT45ʹ�õ���������
//%$#@num,-1,1024,
//%$#@enum,true,false,
#define CFG_AT45_PART_FORMAT               false      //����ѡ��,�Ƿ���Ҫ������оƬ��
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

/*------------------------Time Symbol--------------------------------
Symbol  Parameter                               Typ     Max     Units
fSCK    SCK Frequency                                   66      MHz
fCAR1   SCK Frequency for Continuous Array Read         66      MHz
fCAR2   SCK Frequency for Continuous Array Read
        (Low Frequency)                                 33      MHz
tXFR    Page to Buffer Transfer Time                    200     ��s
tcomp   Page to Buffer Compare Time                     200     ��s
tEP     Page Erase and Programming Time         17      40      ms
tP      Page Programming Time                   3       6       ms
tPE     Page Erase Time                         15      35      ms
tBE     Block Erase Time                        45      100     ms
-------------------------------------------------------------------*/

//Read Commands
#define AT45_MainMem_Page_Read          0xD2
#define AT45_Continous_Array_Read_LC    0xE8        //(Legacy Command) up to 66M
#define AT45_Continous_Array_Read_LF    0x03        //(Low Frequency) up to 33M
#define AT45_Continous_Array_Read_HF    0x0B        //(High Frequency) up to 66M
#define AT45_Buff1_Read_LF              0xD1        //(Low Frequency)
#define AT45_Buff2_Read_LF              0xD3        //(Low Frequency)
#define AT45_Buff1_Read                 0xD4
#define AT45_Buff2_Read                 0xD6
//Program and Erase Commands
#define AT45_Buff1_Write                0x84
#define AT45_Buff2_Write                0x87
#define AT45_Buff1_to_MainMem_Page_Prog_vsErase     0x83    //Buffer 1 to Main Memory Page Program with Built-in Erase
#define AT45_Buff2_to_MainMem_Page_Prog_vsErase     0x86    //Buffer 2 to Main Memory Page Program with Built-in Erase
#define AT45_Buff1_to_MainMem_Page_Prog_noErase     0x88    //Buffer 1 to Main Memory Page Program without Built-in Erase
#define AT45_Buff2_to_MainMem_Page_Prog_noErase     0x89    //Buffer 2 to Main Memory Page Program without Built-in Erase
#define AT45_Page_Erase_Cmd             0x81
#define AT45_Block_Erase_Cmd            0x50
#define AT45_Sector_Erase               0x7C
#define AT45_Chip_Erase1                0xC7
#define AT45_Chip_Erase2                0x94
#define AT45_Chip_Erase3                0x80
#define AT45_Chip_Erase4                0x9A
#define AT45_MainMem_Page_Prog_Through_Buff1        0x82
#define AT45_MainMem_Page_Prog_Through_Buff2        0x85
//Additional Commands
#define AT45_MainMem_Page_to_Buff1_Transfer         0x53
#define AT45_MainMem_Page_to_Buff2_Transfer         0x55
#define AT45_MainMem_Page_to_Buff1_Compare          0x60
#define AT45_MainMem_Page_to_Buff2_Compare          0x61
#define AT45_Auto_Page_Rewrite_through_Buff1        0x58
#define AT45_Auto_Page_Rewrite_through_Buff2        0x59
#define AT45_Deep_Power_down                        0xB9
#define AT45_Resume_Power_down                      0xAB
#define AT45_Status_Register_Read                   0xD7
#define AT45_Command_ID                             0x9F    //Manufacturer and Device ID Read
//AT45_Status_Register bit define
#define AT45_Status_Reg_Bit_BUSY        0x80    //1=Ready   0=Busy
#define AT45_Status_Reg_Bit_COMP        0x40    //1=not match   0=match
#define AT45_Status_Reg_Bit_PRTC        0x02    //1=En protect  0=Dis protect
#define AT45_Status_Reg_Bit_PGSZ        0x01    //1=page size 512   0=page size 528

//#ifndef CFG_AT45_PAGE_SIZE
////CFG_AT45_PAGE_SIZEĬ��ֵΪ528���粻ʹ��Ĭ��ֵ������config-prj.h�ж���
//#define CFG_AT45_PAGE_SIZE      528
//#endif

#define AT45_Block_Size     CFG_AT45_PAGE_SIZE*8

//buff num define
#define AT45_Buff1  1
#define AT45_Buff2  2
//page erase define
#define vs_Erase    1
#define no_Erase    2

struct SPI_Device *s_ptAT45_Dev;
u32 AT45_OP_TIMEOUT = 1800000; // 30����
#define AT45_SPI_SPEED      (10*1000*1000)
static bool_t sAT45Inited = false;

//Commandָ�����
u8 _at45db321_Command[10]={0};
//SPI FLASH���ݻ�����
u8 _at45db321_buff[CFG_AT45_PAGE_SIZE] = {0};
//��ǰ������д���ݵ�Buff,������һBuff�п�����������FLASHд���ݵĽ׶�
u8 _at45db321_Ready_Buff = AT45_Buff1;

struct MutexLCB *pAT45_Lock;   //оƬ������ʱ���
struct NorDescr *nordescription;
char *At45Name = "AT45DB321E";
struct umedia *at45_umedia;
extern struct Object *s_ptDeviceRoot;

s32 __at45_req(enum ucmd cmd, ptu32_t args, ...);
s32 __at45_erase(u32 unit, struct uesz sz);
bool_t at45db321_Wait_Ready(u32 Time_Out);
/*---------------------test use only----------------------
#define test_buff_size  10240
section("seg_int_data") u32 Data_write[test_buff_size]={0};
section("seg_int_data") u32 Data_read[test_buff_size]={0};
u32 buff1_cnt=0,buff2_cnt=0;
---------------------test use only----------------------*/
bool_t _at45db321_cs_active(void)
{
    return SPI_CsActive(s_ptAT45_Dev,AT45_OP_TIMEOUT);
}
void _at45db321_cs_inactive(void)
{
    SPI_CsInactive(s_ptAT45_Dev);
    DJY_DelayUs(20);
}
bool_t _at45db321_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen)
{
    struct SPI_DataFrame data;

    data.RecvBuf = rdata;
    data.RecvLen = rlen;
    data.RecvOff = slen;
    data.SendBuf = sdata;
    data.SendLen = slen;

    return SPI_Transfer(s_ptAT45_Dev,&data,true,AT45_OP_TIMEOUT);
}

//note: Ӧ�ó����е�Addressʵ��Ϊ�����ַ����Ҫ����ת�����ܳ�ΪAT45�е�ʵ�ʵ�ַ
//      AT45�õ�ַ��A0-A9����Page�ڵ�ƫ�Ƶ�ַ��A10-A23����Page��ַ������ΪAT45��Page��С��528bytes
//      �����ô˱�ʾ��ʵ�������ַ�ǲ������ģ�����Ӧ�ó�����ʹ�õ������������ַ��Ҫ�����任���ɳ�Ϊʵ�������ַ

//----ͨ�������ַ����Page��ƫ�Ƶ�ַ-------------------------------------------
//���ܣ�ͨ�������ַ����Page��ƫ�Ƶ�ַ
//      Address(23bit) = page_addr(13bit) + byte_offset_addr(10bit for 528)
//      1 page = 528 bytes
//������Address �����ַ
//���أ�Page��ƫ�Ƶ�ַ��ַbyte_offset_addr
//-----------------------------------------------------------------------------
u32 _at45db321_Offset_Caculate(u32 Address)
{
#if CFG_AT45_PAGE_SIZE==512
    return ( (Address%CFG_AT45_PAGE_SIZE)&0x1FF );
#elif CFG_AT45_PAGE_SIZE==528
    return ( (Address%CFG_AT45_PAGE_SIZE)&0x3FF );
#endif
}

//----ͨ�������ַ����Page��ַ------------------------------------------------
//���ܣ�ͨ�������ַ����Page��ַ
//      Address(23bit) = page_addr(13bit) + byte_offset_addr(10bit for 528)
//      1 page = 528 bytes
//������Address �����ַ
//���أ�Page��ַ
//-----------------------------------------------------------------------------
u32 _at45db321_Page_Caculate(u32 Address)
{
    return (u32)( Address/CFG_AT45_PAGE_SIZE );
}

//----ͨ��Page��ַ����Block��ַ------------------------------------------------
//���ܣ�ͨ��Page��ַ����Block��ַ
//      page_addr = block_addr(10bit) + page_offset_addr(3bit)
//      1 block = 8 pages
//������page_addr   Page��ַ
//���أ�Block��ַ
//-----------------------------------------------------------------------------
u32 _at45db321_Block_Caculate(u32 page_addr)
{
    return ( page_addr>>3 );
}

//----д�����ݳ��ȼ���--------------------------------------------------------
//���ܣ�ͨ���Ƚ�Page�Ĵ�Сȷ�����ο���д������ݳ���
//������byte_offset_addr    Page�ڵ�ƫ�Ƶ�ַ
//      data_len            ����д������ݳ���
//���أ�����ʵ���ܹ�д������ݳ���
//-----------------------------------------------------------------------------
u32 _at45db321_Written_Caculate(u32 byte_offset_addr,u32 data_len)
{
    u32 written_data_len;
    written_data_len = CFG_AT45_PAGE_SIZE-byte_offset_addr;
    if( written_data_len>data_len )
        written_data_len = data_len;
    return written_data_len;
}

//----SPI FLASH�������ݶ�ȡ----------------------------------------------------
//���ܣ���AT45�ĵ�ַAddress����ʼ��������ȡdata_len���ȵ����ݣ�����data
//      ����ַ�ۼƵ�FLASHĩβ�򷵻ؿ�ͷ��������ȡ
//������page_addr   Page��ַ
//      byte_offset_addr    Page�ڵ�ƫ�Ƶ�ַ
//      *data       ���ݴ洢�׵�ַ
//      data_len    ���ݳ���
//���أ�ʵ�ʶ�ȡ�����ݳ���
//-----------------------------------------------------------------------------
u32 _at45db321_Continuous_Array_Read(u32 page_addr,u32 byte_offset_addr,u8 *data,u32 data_len)
{
    //array_addr
    _at45db321_Command[0] = AT45_Continous_Array_Read_HF;
#if CFG_AT45_PAGE_SIZE==512
    _at45db321_Command[1] = (page_addr>>7)&0xFF;
    _at45db321_Command[2] = ((page_addr<<1)|(byte_offset_addr>>8))&0xFF;
#elif CFG_AT45_PAGE_SIZE==528
    _at45db321_Command[1] = (page_addr>>6)&0xFF;
    _at45db321_Command[2] = ((page_addr<<2)|(byte_offset_addr>>8))&0xFF;
#endif
    _at45db321_Command[3] = byte_offset_addr & 0xFF;
    _at45db321_Command[4] = 0xFF;

    if(false == _at45db321_cs_active())
        return 0;

    if(_at45db321_TxRx(_at45db321_Command,5,data,data_len))
    {
        _at45db321_cs_inactive();
        return 0;
    }
    _at45db321_cs_inactive();
    return data_len;
}

//----SPI FLASHд���ݵ�������--------------------------------------------------
//���ܣ�SPI FLASHд���ݵ�������
//������buff_num    Ŀ�껺����ѡ��
//      buff_addr   ��������ƫ�Ƶ�ַ
//      *data       ���ݻ�ȡ�׵�ַ
//      data_len    ���ݳ���
//���أ�ʵ��д������ݳ���
//-----------------------------------------------------------------------------
u32 _at45db321_Buff_Write(u32 buff_num,u32 buff_addr,u8 *data,u32 data_len)
{
    if(buff_num == AT45_Buff1)
        _at45db321_Command[0] = AT45_Buff1_Write;
    else if(buff_num == AT45_Buff2)
        _at45db321_Command[0] = AT45_Buff2_Write;
    else
    {
        return 0;
    }
    //buff_addr
    _at45db321_Command[1] = 0xFF;
    _at45db321_Command[2] = (buff_addr>>8)&0xFF;
    _at45db321_Command[3] = (buff_addr)&0xFF;

    if(FALSE ==_at45db321_cs_active())
    {
        return 0;
    }

    if(_at45db321_TxRx(_at45db321_Command,4,NULL,0))
    {
        _at45db321_cs_inactive();
        return 0;
    }
    if(_at45db321_TxRx(data,data_len,NULL,0))
    {
        _at45db321_cs_inactive();
        return 0;
    }
    _at45db321_cs_inactive();

    return data_len;
}

//----SPI FLASH������������----------------------------------------------------
//���ܣ�SPI FLASH������������
//������buff_num    Ŀ�껺����ѡ��
//      buff_addr   ��������ƫ�Ƶ�ַ
//      *data       ���ݽ����׵�ַ
//      data_len    ���ݳ���
//���أ�ʵ�ʶ�ȡ�����ݳ���
//-----------------------------------------------------------------------------
u32 _at45db321_Buff_Read(u32 buff_num,u32 buff_addr,u8 *data,u32 data_len)
{
    if(buff_num == AT45_Buff1)
        _at45db321_Command[0] = AT45_Buff1_Read;
    else if(buff_num == AT45_Buff2)
        _at45db321_Command[0] = AT45_Buff2_Read;
    else
    {
        return 0;
    }
    //buff_addr
    _at45db321_Command[1] = 0xFF;
    _at45db321_Command[2] = (buff_addr>>8)&0xFF;
    _at45db321_Command[3] = (buff_addr)&0xFF;
    _at45db321_Command[4] = 0xFF;

    if(false == _at45db321_cs_active())
        return 0;

    _at45db321_TxRx(_at45db321_Command,5,data,data_len);

    _at45db321_cs_inactive();

    return data_len;
}

//----SPI FLASH��Page���ݶ���������--------------------------------------------
//���ܣ�SPI FLASH��Page���ݶ���������
//������buff_num    Ŀ�껺����ѡ��
//      page_addr   Ŀ��Page��ַ
//���أ�false:  ʧ��
//      true:   �ɹ�
//-----------------------------------------------------------------------------
bool_t _at45db321_Page_to_Buff(u32 buff_num,u32 page_addr)
{
    if(buff_num == AT45_Buff1)
        _at45db321_Command[0] = AT45_MainMem_Page_to_Buff1_Transfer;
    else if(buff_num == AT45_Buff2)
        _at45db321_Command[0] = AT45_MainMem_Page_to_Buff2_Transfer;
    else
    {
        return 0;
    }
#if CFG_AT45_PAGE_SIZE==512
    _at45db321_Command[1] = (page_addr>>7)&0xFF;
    _at45db321_Command[2] = (page_addr<<1)&0xFF;
#elif CFG_AT45_PAGE_SIZE==528
    _at45db321_Command[1] = (page_addr>>6)&0xFF;
    _at45db321_Command[2] = (page_addr<<2)&0xFF;
#endif
    _at45db321_Command[3] = 0x0;

    if(false == _at45db321_cs_active())
        return 0;

    if(_at45db321_TxRx(_at45db321_Command,4,NULL,0))
    {
        _at45db321_cs_inactive();
        return 0;
    }

    _at45db321_cs_inactive();
    return true;
}

//----SPI FLASH������������д��Page--------------------------------------------
//���ܣ�SPI FLASH������������д��Page
//������buff_num    Ŀ�껺����ѡ��
//      page_addr   Ŀ��Page��ַ
//      With_Erase  дPage֮ǰ�Ƿ���Ҫ����
//���أ�false:  ʧ��
//      true:   �ɹ�
//-----------------------------------------------------------------------------
bool_t _at45db321_Buff_to_Page(u32 buff_num,u32 page_addr,u32 With_Erase)
{
    if(buff_num == AT45_Buff1)
    {
        if(With_Erase == no_Erase)
            _at45db321_Command[0] = AT45_Buff1_to_MainMem_Page_Prog_noErase;
        else if(With_Erase == vs_Erase)
            _at45db321_Command[0] = AT45_Buff1_to_MainMem_Page_Prog_vsErase;
        else
        {
            return 0;
        }
    }
    else if(buff_num == AT45_Buff2)
    {
        if(With_Erase == no_Erase)
            _at45db321_Command[0] = AT45_Buff2_to_MainMem_Page_Prog_noErase;
        else if(With_Erase == vs_Erase)
            _at45db321_Command[0] = AT45_Buff2_to_MainMem_Page_Prog_vsErase;
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }

#if CFG_AT45_PAGE_SIZE==512
    _at45db321_Command[1] = (page_addr>>7)&0xFF;
    _at45db321_Command[2] = (page_addr<<1)&0xFF;
#elif CFG_AT45_PAGE_SIZE==528
    _at45db321_Command[1] = (page_addr>>6)&0xFF;
    _at45db321_Command[2] = (page_addr<<2)&0xFF;
#endif
    _at45db321_Command[3] = 0x0;

    if(false == _at45db321_cs_active())
        return 0;

    if(_at45db321_TxRx(_at45db321_Command,4,NULL,0))
    {
        _at45db321_cs_inactive();
        return 0;
    }

    _at45db321_cs_inactive();
    return true;
}

//----SPI FLASH Page���������ж�-----------------------------------------------
//���ܣ��ж�FLASH�����е���������Ҫд������ݶ�֮���ƥ��ȣ��ж��Ƿ���Ҫ����
//      FLASH��ӦPAGE����д����Ҫд��������Ѿ�д��Buff��Flash�ж�����������_at45db321_buff��
//������*data       ��Ҫд������ݵ���ʼ��ַ
//      data_len    ��Ҫд������ݳ���
//���أ�false:  ��Ҫ����
//      true:   ����Ҫ����
//-----------------------------------------------------------------------------
bool_t _at45db321_Need_Erase_orNot(u8 *data,u32 data_len)
{
    u32 i;

    for(i=0;i<data_len;i++)
    {
        if(_at45db321_buff[i] != 0xFF)
            return false;
    }

    return true;
}

//----SPI FLASH Page����-------------------------------------------------------
//���ܣ�SPI FLASH Page����
//��������
//���أ���
//-----------------------------------------------------------------------------

bool_t at45db321_Wait_Ready_erase(u32 Time_Out);
bool_t AT45_Page_Erase(u32 Address)
{
    u32 page_addr;

    if(false == Lock_MutexPend(pAT45_Lock, AT45_OP_TIMEOUT))
    {
        return false;
    }

    if(false == at45db321_Wait_Ready_erase(500000))       //��æ������ʱ�򷵻�false
        return false;//��ʱ���˳�

    page_addr = _at45db321_Page_Caculate(Address);

    _at45db321_Command[0] = AT45_Page_Erase_Cmd;
#if CFG_AT45_PAGE_SIZE==512
    _at45db321_Command[1] = (page_addr>>7)&0xFF;
    _at45db321_Command[2] = (page_addr<<1)&0xFF;
#elif CFG_AT45_PAGE_SIZE==528
    _at45db321_Command[1] = (page_addr>>6)&0xFF;
    _at45db321_Command[2] = (page_addr<<2)&0xFF;
#endif
    _at45db321_Command[3] = 0xFF;

    if(false == _at45db321_cs_active())
        return 0;

    _at45db321_TxRx(_at45db321_Command,4,NULL,0);

    _at45db321_cs_inactive();

    DJY_EventDelay(50000);
    Lock_MutexPost(pAT45_Lock);
    return true;
}

//----SPI FLASH Block����------------------------------------------------------
//���ܣ�SPI FLASH Block����
//��������
//���أ���
//-----------------------------------------------------------------------------
s32 AT45_Block_Erase(u32 Address)
{
    u32 block_addr;
    s32 res;
    struct SPI_DataFrame frame;
    u8 command[] = {0, 0 ,0 ,0};
    if(false == Lock_MutexPend(pAT45_Lock, AT45_OP_TIMEOUT))
    {
        return -1;
    }

    block_addr = _at45db321_Page_Caculate(Address);
    block_addr = _at45db321_Block_Caculate(block_addr);

    command[0] = AT45_Block_Erase_Cmd;
    command[1] = (block_addr>>4)&0x3F;
    command[2] = (block_addr<<4)&0xF0;
    command[3] = 0x0;

    frame.RecvBuf = NULL;
    frame.RecvLen = 0;
    frame.RecvOff = 0;
    frame.SendBuf = command;
    frame.SendLen = 4;

    if(false == at45db321_Wait_Ready(500000))       //��æ������ʱ�򷵻�false
    {
        Lock_MutexPost(pAT45_Lock);
        return -1;//��ʱ���˳�
    }

    if(FALSE == _at45db321_cs_active())
    {
        Lock_MutexPost(pAT45_Lock);
        return -1;//��ʱ���˳�
    }
    res = SPI_Transfer(s_ptAT45_Dev, &frame, TRUE, AT45_OP_TIMEOUT);

    _at45db321_cs_inactive();

    Lock_MutexPost(pAT45_Lock);

    return res;
}

//----SPI FLASHȫƬ����--------------------------------------------------------
//���ܣ�SPI FLASHȫƬ����
//��������
//���أ���
//-----------------------------------------------------------------------------
bool_t AT45_Chip_Erase(void)
{
    if(false == Lock_MutexPend(pAT45_Lock, AT45_OP_TIMEOUT))
    {
        return false;
    }

    _at45db321_Command[0] = AT45_Chip_Erase1;
    _at45db321_Command[1] = AT45_Chip_Erase2;
    _at45db321_Command[2] = AT45_Chip_Erase3;
    _at45db321_Command[3] = AT45_Chip_Erase4;

    if(false == at45db321_Wait_Ready(500000))       //��æ������ʱ�򷵻�false
        return false;//��ʱ���˳�

    _at45db321_cs_active();

    _at45db321_TxRx(_at45db321_Command,4,NULL,0);

    _at45db321_cs_inactive();

    DJY_EventDelay(100000000);
    Lock_MutexPost(pAT45_Lock);

    return true;
}

//----SPI FLASH Page Size����--------------------------------------------------
//���ܣ�SPI FLASH Page Size����Ϊ512�����ñ���������Ҫ�����ϵ磬�������FLASH��õ���������
//      ע�⣺�ù��̲����棡��AT45Ĭ��PageSizeΪ528bytes�����ñ����������޸�Ϊ512
//��������
//���أ���
//-----------------------------------------------------------------------------
void _at45db321_Binary_Page_Size_512(void)
{
    _at45db321_Command[0] = 0x3D;
    _at45db321_Command[1] = 0x2A;
    _at45db321_Command[2] = 0x80;
    _at45db321_Command[3] = 0xA6;

    _at45db321_cs_active();

    _at45db321_TxRx(_at45db321_Command,4,NULL,0);

    _at45db321_cs_inactive();

//  while(1);
}

//----SPI FLASH��ȡStatus Register---------------------------------------------
//���ܣ�SPI FLASH��ȡStatus Registerֵ
//��������
//���أ�Status Registerֵ
//-----------------------------------------------------------------------------
u32 _at45db321_Read_Status(void)
{
    u8 Data;

    _at45db321_Command[0] = AT45_Status_Register_Read;

    _at45db321_cs_active();

    _at45db321_TxRx(_at45db321_Command,1,&Data,1);

    _at45db321_cs_inactive();

    return Data;
}

void _at45db321_Check_SP(void)
{
    u8 Data[64],i;

    if(false == at45db321_Wait_Ready(500000))       //��æ������ʱ�򷵻�false
        return ;//��ʱ���˳�

    _at45db321_Command[0] = 0x32;
    _at45db321_Command[1] = 0xFF;
    _at45db321_Command[2] = 0xFF;
    _at45db321_Command[3] = 0xFF;


    _at45db321_cs_active();
    _at45db321_TxRx(_at45db321_Command,4,Data,64);
    _at45db321_cs_inactive();

    for(i = 0; i < 64; i ++)
    {
        if(Data[i] != 0)
            break;
    }

    if(i != 64)
    {

        if(false == at45db321_Wait_Ready(500000))       //��æ������ʱ�򷵻�false
            return ;//��ʱ���˳�

        //erase sector protect
        _at45db321_Command[0] = 0x3D;
        _at45db321_Command[1] = 0x2A;
        _at45db321_Command[2] = 0x7F;
        _at45db321_Command[3] = 0xCF;



        _at45db321_cs_active();
        _at45db321_TxRx(_at45db321_Command,4,NULL,0);
        _at45db321_cs_inactive();

        //programe sector protect
        memset(Data,0x00,64);
        if(false == at45db321_Wait_Ready(500000))       //��æ������ʱ�򷵻�false
            return ;//��ʱ���˳�
        _at45db321_Command[0] = 0x3D;
        _at45db321_Command[1] = 0x2A;
        _at45db321_Command[2] = 0x7F;
        _at45db321_Command[3] = 0xFC;

        _at45db321_cs_active();
        _at45db321_TxRx(_at45db321_Command,4,NULL,0);
        _at45db321_TxRx(Data,64,NULL,0);
        _at45db321_cs_inactive();

        if(false == at45db321_Wait_Ready(500000))       //��æ������ʱ�򷵻�false
            return ;//��ʱ���˳�
        //check it
        _at45db321_Command[0] = 0x32;
        _at45db321_Command[1] = 0xFF;
        _at45db321_Command[2] = 0xFF;
        _at45db321_Command[3] = 0xFF;

        _at45db321_cs_active();
        _at45db321_TxRx(_at45db321_Command,4,Data,64);
        _at45db321_cs_inactive();
    }

}

void _at45db321_Check_Lock(void)
{
    u8 Data[64];

    _at45db321_Command[0] = AT45_Status_Register_Read;
    _at45db321_cs_active();
    _at45db321_TxRx(_at45db321_Command,1,Data,2);
    _at45db321_cs_inactive();

    if(Data[1] & (0x08))//means locked down
    {
        //then read it
        _at45db321_Command[0] = 0x35;
        _at45db321_Command[1] = 0xFF;
        _at45db321_Command[2] = 0xFF;
        _at45db321_Command[3] = 0xFF;


        _at45db321_cs_active();
        _at45db321_TxRx(_at45db321_Command,4,Data,64);
        _at45db321_cs_inactive();
    }
}
//----SPI FLASHУ��оƬID------------------------------------------------------
//���ܣ�SPI FLASHУ��оƬID
//��������
//���أ�true = У��������false = У�鲻�ɹ�
//-----------------------------------------------------------------------------
bool_t _at45db321_Check_ID(void)
{
    u8 Data[5];

    _at45db321_Command[0] = AT45_Command_ID;

    _at45db321_cs_active();

    _at45db321_TxRx(_at45db321_Command,1,Data,5);

    _at45db321_cs_inactive();

    if( (Data[0] != 0x1F) ||            //Manufacturer_ID
        (Data[1] != 0x27) ||            //Device_ID1
        (Data[2] != 0x01) ||            //Device_ID2
        (Data[3] != 0x01) ||
        (Data[4] != 0x00))              //Extended_Info
    {
        return false;
    }

    return true;    //Match SPI Flash ID successful
}

/*--------------------Ӧ�ó�����ýӿں��������ɴ˿�ʼ------------------------*/

//----SPI FLASHģ���æ--------------------------------------------------------
//���ܣ�SPI FLASHģ���æ����ȡAT45_Status������BUSY��־λ�ĵ�ǰֵ
//��������
//���أ�true = FLASH���У�false = FLASHæ
//-----------------------------------------------------------------------------
bool_t at45db321_Check_Busy(void)
{
    u8 Data;

    Data = _at45db321_Read_Status();

    if( AT45_Status_Reg_Bit_BUSY != (AT45_Status_Reg_Bit_BUSY & Data) )
        return false;
    else
        return true;
}
//----�ȴ�FLASHģ�����--------------------------------------------------------
//���ܣ��ȴ�FLASHģ����У������ڲ��ж��Ƿ�ʱ
//��������
//���أ�true = FLASH���У�false = FLASH��ʱ�쳣
//-----------------------------------------------------------------------------
bool_t at45db321_Wait_Ready(u32 Time_Out)
{
    u8 Data[2],result = true;


    _at45db321_Command[0] = AT45_Status_Register_Read;

    do
    {
        _at45db321_cs_active();
        _at45db321_TxRx(_at45db321_Command,1,Data,2);
        _at45db321_cs_inactive();

        Time_Out -= 2;
        DJY_DelayUs(2);
        if(Time_Out == 0)
        {
            result = false;
            break;
        }
    }while( AT45_Status_Reg_Bit_BUSY != (AT45_Status_Reg_Bit_BUSY & Data[0]) );

    return result;
}

bool_t at45db321_Wait_Ready_erase(u32 Time_Out)
{
    u8 Data[2],result = true;

    if(Time_Out < 1000)
        Time_Out = 1000;
    _at45db321_Command[0] = AT45_Status_Register_Read;

    do
    {
        _at45db321_cs_active();
        _at45db321_TxRx(_at45db321_Command,1,Data,2);
        _at45db321_cs_inactive();

        Time_Out -= 1000;
        DJY_EventDelay(1*mS);
        if(Time_Out == 0)
        {
            result = false;
            break;
        }
    }while( AT45_Status_Reg_Bit_BUSY != (AT45_Status_Reg_Bit_BUSY & Data[0]) );

    return result;
}

//----SPI FLASHģ�������------------------------------------------------------
//���ܣ���SPI FLASH��Address��ַ����ʼ������data_len���ȵ����ݣ����ݴ洢�׵�ַΪdata
//      ����ַ�ۼӵ�FLASH��ĩβ����᷵��FLASH��ͷ��������ȡ��ֱ������data_len������Ϊֹ
//������Address     ��Ҫд��ĵ�ַ��ע�⣬�˵�ַΪӦ�ó�����ʹ�õ������ַ����Ҫ�����任��Ϊ�����ַ��page_addr��byte_offset_addr��
//      *data       ��ȡ���ݴ洢λ�õ���ʼ��ַ
//      data_len    ��ȡ���ݳ���
//���أ�data_len = ʵ�ʶ�ȡ�����ݳ��ȣ�false = ��ȡʧ��
//-----------------------------------------------------------------------------
u32 AT45_FLASH_Read(u32 Address,u8 *data,u32 data_len)
{
    u32 page_addr,byte_offset_addr;

    ////�ж��Ƿ���Ҫ��ʼ��SPI���Է�module_init_at45db321δ������
    if(!sAT45Inited)
    {
        return 0;
    }

    if(false == Lock_MutexPend(pAT45_Lock, AT45_OP_TIMEOUT))
    {
        return 0;
    }

    page_addr=_at45db321_Page_Caculate(Address);
    byte_offset_addr=_at45db321_Offset_Caculate(Address);

    if(false == at45db321_Wait_Ready(500000))       //��æ������ʱ�򷵻�false
    {
        Lock_MutexPost(pAT45_Lock);
        return false;//��ʱ���˳�
    }

    _at45db321_Continuous_Array_Read(page_addr,byte_offset_addr,data,data_len);

//    DJY_EventDelay(10000);
    Lock_MutexPost(pAT45_Lock);

    return data_len;
}

//----SPI FLASHģ��д����------------------------------------------------------
//���ܣ���SPI FLASH��Address��ַ����ʼ��д��data_len���ȵ����ݣ����ݻ�ȡ�׵�ַΪdata
//      ��������д�������ġ����ݳ���Ϊ���Pageʱ����ʹFLASH��Ч�ʴﵽ���š�
//      Ӧ�ó����������FLASH�Ƿ���Ҫ��������д��
//      ע�⣺Ӧ�ó���ϣ��д������ݳ���Ϊdata_len���������򲻱�֤ʵ��д��data_len���ȵ����ݣ�
//      ʵ��д������ݳ��Ȼ�ͨ��written_data_len���ظ�Ӧ�ó��򣨸���Page��С�����ģ�
//������Address     ��Ҫд��ĵ�ַ��ע�⣬�˵�ַΪӦ�ó�����ʹ�õ������ַ��
//      *data       д������Դ����ʼ��ַ
//      data_len    д�����ݳ���
//���أ�written_data_len = ʵ��д������ݳ��ȣ�false = д��ʧ��
//-----------------------------------------------------------------------------
u32 _at45_flash_write(u32 Address,u8 *data,u32 data_len)
{
    u32 written_data_len=0,Erase_orNot=vs_Erase;
    u32 page_addr,byte_offset_addr;

    //����Page��ַ��������Page�е�ƫ�Ƶ�ַ
    page_addr = _at45db321_Page_Caculate(Address);
    byte_offset_addr = _at45db321_Offset_Caculate(Address);

    //ȷ�ϱ��ο���д������ݳ���
    written_data_len = _at45db321_Written_Caculate(byte_offset_addr,data_len);

    //1.���ȣ�����Ҫд�������д�봦�ڿ���״̬��Buff��
    if( false == at45db321_Check_Busy() )
    {//��ǰBuff����ִ��д��FLASH�Ĳ�������һ��Buffд������
        if( _at45db321_Ready_Buff==AT45_Buff1 )
            _at45db321_Ready_Buff = AT45_Buff2;
        else
            _at45db321_Ready_Buff = AT45_Buff1;
    }
    _at45db321_Buff_Write(_at45db321_Ready_Buff,byte_offset_addr,data,written_data_len);

    if(false == at45db321_Wait_Ready(500000))       //��æ������ʱ�򷵻�false
        return false;//��ʱ���˳�

    //2.��Σ���FLASH���ж����Ѿ����ڵ�����,�ж��Ƿ���Ҫ����
    _at45db321_Continuous_Array_Read(page_addr,byte_offset_addr,_at45db321_buff,written_data_len);//����������_at45db321_buff
    if( false == _at45db321_Need_Erase_orNot(data,written_data_len))
    {//��Ҫ����
        Erase_orNot = vs_Erase;
    }
    else
    {//����Ҫ����
        Erase_orNot = no_Erase;
    }

    //3.������д�����
    //д��ʱ�����뽫flash�е����ݶ��뵽Ƭ��RAM�����򣬽��ὫRAM��δȷ��������ˢ�뵽FLASH
//  if((( byte_offset_addr != 0 )||( written_data_len != CFG_AT45_PAGE_SIZE ))&&(Erase_orNot == vs_Erase))
    {//������Page��д�룬����ǰ��Ҫ����
        _at45db321_Page_to_Buff(_at45db321_Ready_Buff,page_addr);
        if(false == at45db321_Wait_Ready(500000))       //��æ������ʱ�򷵻�false
            return false;//��ʱ���˳�
        _at45db321_Buff_Write(_at45db321_Ready_Buff,byte_offset_addr,data,written_data_len);
    }

    _at45db321_Buff_to_Page(_at45db321_Ready_Buff,page_addr,Erase_orNot);
    //�˴�������Ҫ�ȴ���ɡ��ڽ�����һ��FLASH����ʱ���ж�

    return written_data_len;
}

// =============================================================================
// ���ܣ�дflash���� ҳд,��data_len������д�뵽Address����ʼ��ַ
// ������Address����ʼ��ַ
//      data������ָ��
//      data_len�����ݳ��ȣ��ֽ���
// ���أ�ʵ��д������ݳ���
// =============================================================================
u32 AT45_FLASH_Write(u32 Address,u8 *data,u32 data_len)
{
    u32 wsize,temp;
    ////�ж��Ƿ���Ҫ��ʼ��SPI���Է�module_init_at45db321δ������
    if(!sAT45Inited)
    {
        return false;
    }
    if(false == Lock_MutexPend(pAT45_Lock, AT45_OP_TIMEOUT))
    {
        return false;
    }
    temp = data_len;
    while(temp)
    {
        wsize = _at45_flash_write(Address,data,temp);
        if(!wsize)
            break;
        Address += wsize;
        data    += wsize;
        temp -= wsize;
    }
    DJY_EventDelay(10000);
    Lock_MutexPost(pAT45_Lock);
    return data_len - temp;
}

// =============================================================================
// ���ܣ�����AT45DB321�Ƿ��Ѿ���ʼ��
// ��������
// ���أ���ʼ��״̬
// =============================================================================
bool_t AT45_FLASH_Ready(void)
{
    return sAT45Inited;
}

// =============================================================================
// ���ܣ���װat45����
// ������
// ���أ��ɹ���true����ʧ�ܣ�false����
// ��ע��
// =============================================================================
bool_t ModuleInstall_at45db321(void)
{
    pAT45_Lock = Lock_MutexCreate("AT45 Lock");
    if(!pAT45_Lock)
    {
        printf("\r\n: error : device : cannot create AT45 flash lock.");
        return false;
    }

    s_ptAT45_Dev = SPI_DevAdd(CFG_AT45_BUSNAME,At45Name,0,8,SPI_MODE_0,SPI_SHIFT_MSB,AT45_SPI_SPEED,false);
    if(s_ptAT45_Dev != NULL)
    {
        SPI_BusCtrl(s_ptAT45_Dev, CN_SPI_SET_POLL, 0, 0);
    }
    else
    {
        printf("\r\n: error  : device : AT45DB321E init failed.\n\r");
        return false;
    }

    if(false == _at45db321_Check_ID())  //У��оƬID
        return false;

    if( !(_at45db321_Read_Status() & AT45_Status_Reg_Bit_PGSZ) )//ת����512�ֽ�
    {
        _at45db321_Binary_Page_Size_512();//�����棬���������ϵ�
        printf("\r\n: info  : device : AT45DB321 page size �ı䣬�������ϵ�\n\r");
    }

    if(!nordescription) //��ʼ��nor����Ϣ
    {
        nordescription = malloc(sizeof(struct NorDescr));
        if(!nordescription)
        {
            printf("\r\n: erro : device : memory out.\r\n");
            return false;
        }

        memset(nordescription, 0x0, (sizeof(struct NorDescr)));

        // AT45��sector��block�󣬶���sector�Ĵ�С��һ�¡������߼��Ͼͽ�sector����page��
        // ��Ȼsector,block���
        nordescription->PortType = NOR_SPI;
        nordescription->Port = s_ptAT45_Dev;
        nordescription->BytesPerPage = CFG_AT45_PAGE_SIZE;
        nordescription->PagesPerSector = 1;
        nordescription->SectorsPerBlk = 8;
        nordescription->Blks = 1024; // ȫ������������
        nordescription->ReservedBlks = 0;
    }

    if(CFG_AT45_PART_FORMAT)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        if(-1 == __at45_req(format, 0 , -1, &sz))
        {
            warning_printf("at45"," Format failure.");
        }
    }

    at45_umedia = malloc(sizeof(struct umedia)+nordescription->BytesPerPage);
    if(!at45_umedia)
        return false;

    at45_umedia->mreq = __at45_req;
    at45_umedia->type = nor;
    at45_umedia->ubuf = (u8*)at45_umedia + sizeof(struct umedia);

    if(!Device_Create((const char*)At45Name, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)at45_umedia)))
    {
        printf("\r\n: erro : device : %s addition failed.", At45Name);
        free(at45_umedia);
        return false;
    }

    sAT45Inited = true;
    return true;
}
// =============================================================================
// ���ܣ��ж�at45�Ƿ�װ
// ������  ��
// ���أ��ѳɹ���װ��true����δ�ɹ���װ��false����
// ��ע��
// =============================================================================
bool_t At45_is_install(void)
{
    return sAT45Inited;
}

//�ļ�ϵͳʹ�õ���ؽӿ�
#if 1
// ============================================================================
// ���ܣ�at45����
// ������cmd -- ���
//      args -- �ɱ�Σ����������
// ���أ�
// ��ע��
// ============================================================================
s32 __at45_req(enum ucmd cmd, ptu32_t args, ...)
{
    s32 res = 0;

    switch(cmd)
    {
        case whichblock:
        {
            va_list list;
            u32 *block;
            s64 *unit;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);
            *block = *unit / nordescription->SectorsPerBlk;     //��ȡ��ǰҳ������һ��
            break;
        }

        case totalblocks:
        {
            *((u32*)args) =  nordescription->Blks;      //1���������ж��ٿ�
            break;
        }

        case blockunits:
        {
            *((u32*)args) = nordescription->SectorsPerBlk;      //1�������ж���ҳ
            break;
        }

        case unitbytes:
        {
            *((u32*)args) = nordescription->BytesPerPage;   //ÿҳ���ֽ���
            break;
        }

        case format:
        {
            va_list list;
            s32 start, end;
            struct uesz *sz;

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            sz = (struct uesz*)va_arg(list, u32);
            va_end(list);

            if(!sz->block)
                return (-1);

            if(-1==end)
                end = nordescription->Blks;

            do
            {
                if(__at45_erase((s64)--end, *sz))
                {
                    res = -1;
                    break;
                }
            }
            while(end!=start);      //����ָ������

            break;
        }

        case remain:
        {
            va_list list;
            u32 *left;
            s64 *unit;

            left = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);
            //��ǰҳ�����Ŀ��ڻ��л��ж���ҳû��
            *left = (nordescription->SectorsPerBlk - 1)- (*unit % nordescription->SectorsPerBlk);

            break;
        }

        case checkbad: break; // ��黵��
        default: res = -1; break;
    }

    return (res);
}
// ============================================================================
// ���ܣ���AT45��ҳ���
// ������unit -- ҳ���߿飻
//      sz -- ҳ���߿飻
// ���أ��ɹ� -- ��0����ʧ�� -- ��-1��
// ��ע��
// ============================================================================
s32 __at45_erase(u32 unit, struct uesz sz)
{
    u32 Address;

    if(sz.unit)
        Address = unit / 8; // ��ǰҳ���ڵڼ���
    else if (sz.block)
        Address = unit;

    Address = (Address << 3) * CFG_AT45_PAGE_SIZE;      //��ȡ��������׵�ַ
    return (AT45_Block_Erase(Address));
}
// ============================================================================
// ���ܣ���AT45
// ������unit -- ҳ�ţ�data --��������ݵĻ��棻
// ���أ��ɹ� -- ��0����ʧ�� -- ��-1��
// ��ע��
// ============================================================================
s32 __at45_read(u32 unit, void *data, struct uopt opt)
{
    s32 res;
    u32 Address;
    Address = unit * CFG_AT45_PAGE_SIZE;    //�����ǰҳ���׵�ַ
    res = AT45_FLASH_Read(Address, (u8*)data, CFG_AT45_PAGE_SIZE);
    if(res != CFG_AT45_PAGE_SIZE)
    {
        return -1;
    }
    return 0;
}

// ============================================================================
// ���ܣ�дAT45
// ������unit -- ҳ�ţ�data -- Ҫд�����ݣ�
// ���أ��ɹ� -- ��0����ʧ�� -- ��-1��
// ��ע�������ļ�ϵͳ��at45д����û��ֱ��������Ľӿڣ���������д�ˡ�������Ϊ�ļ�ϵͳ��ƵĶ�flash��д������
// AT45_FLASH_Write�������ڳ�ͻ���ļ�ϵͳдflashʱÿ��������д�����ݳ��̲�һ�����һ�д��Σ���1ҳ����
// ��һҳ�����Ի������1ҳ�ж��д���ݵ�����������ļ�ϵͳÿ�ζ��ǰ�ҳ�������ģ����ļ�ϵͳ��1ҳд�ڶ���ʱ��
// ���Զ�ƫ�Ƶ���һ��д�����ݵĺ��棬���Ḳ�ǵ�һ��д�����ݣ���һ��д������λ�û��Զ����0XFF��flash��λֻ����1��Ϊ0��.
// ��AT45_FLASH_Write�ӿ���д����ʱ�����ָ���ĵ�ַ��ʼд��д֮ǰ���ָ���ĵ�ַ��ʼ�����ݣ���ָ�����ȣ�������
// ��ε�ַ��Χ������ݲ���ȫFFʱ��Ѹ÷�Χ������Ȼ��д�����ݡ�
// �����ͻ����һ�������ļ�ϵͳ����һ��ҳ��д��Σ�ҳ�ĵ�ַ�ǹ̶��ģ����ļ�ϵͳ�ڶ���д��ҳ��ʱ��Ȼ����AT45_FLASH_Write
// �Ļ���AT45_FLASH_Write���Ȱ����ݶ��������������治��ȫFF����Ϊ�е�һ��д�����ݣ������л����һ�飬�����ᵼ�µ�һ��
// д������û�ˡ�
// ============================================================================
s32 __at45_write(u32 unit, void *data, struct uopt opt)
{
    s32 res;
    struct SPI_DataFrame frame;
    u8 command[] = {0, 0 ,0 ,0};

    command[0] = JEDEC_PAGE_WRITE_DIRECT_BY_BUF1;
    command[1] = (unit >> 0x7) & 0x3F;
    command[2] = (unit << 0x1) & 0xFE;
    command[3] = 0x00;

    frame.RecvBuf = NULL;
    frame.RecvLen = 0;
    frame.RecvOff = 0;
    frame.SendBuf = command;
    frame.SendLen = 4;

    if(true == Lock_MutexPend(pAT45_Lock, AT45_OP_TIMEOUT))
    {
        if(true == at45db321_Wait_Ready(500000))       //��æ������ʱ�򷵻�false
        {
            if(true == _at45db321_cs_active())
            {
                res = SPI_Transfer(nordescription->Port, &frame, TRUE, AT45_OP_TIMEOUT);    //�ȷ�������
                if(CN_SPI_EXIT_NOERR == res)
                {
                    frame.RecvBuf = NULL;
                    frame.RecvLen = 0;
                    frame.RecvOff = 0;
                    frame.SendBuf = data;
                    frame.SendLen = nordescription->BytesPerPage;

                    res = SPI_Transfer(nordescription->Port, &frame, TRUE, AT45_OP_TIMEOUT);    //�ٷ���Ҫ�������
//                    if(CN_SPI_EXIT_NOERR == res)
//                        res = nordescription->BytesPerPage;
                }
                _at45db321_cs_inactive();
//                DJY_EventDelay(4000);// ��ʱ�г�. 4ms
//                if(true == at45db321_Wait_Ready(500000))       //��æ������ʱ�򷵻�false
//                {
                Lock_MutexPost(pAT45_Lock);
                if(res != CN_SPI_EXIT_NOERR)
                {
                    return -1;
                }
                return 0;
//                }
            }
            Lock_MutexPost(pAT45_Lock);
        }
    }
    return -1;
}

// ============================================================================
// ���ܣ���ʼ��SPI FLASHģ�飬�����ļ�ϵͳ
// ������   fs -- ��ý����Ҫ��װ�ļ�ϵͳmount�����֣�
//      mediadrv -- ý������
//      dwStart -- ��ʼ�飻
//      dwEnd -- ��������������ʱ�������ÿ飬ֻ�����ÿ����һ�飩��
// ���أ��ɹ���ʼ����0������ʼ��ʧ�ܣ�-1����
// ��ע�������߼������ļ�ϵͳ��ֱ�ӷ����߼��������÷�����
// ============================================================================
s32 __AT45_FsInstallInit(const char *fs, s32 dwStart, s32 dwEnd, void *mediadrv)
{
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res,BlockNum;

    if(mediadrv == NULL)
        return -1;

    targetobj = OBJ_MatchPath(fs, &notfind);    //����mount��������mount���obj
    if(notfind)
    {
        error_printf("at45"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj); //��ȡobj��˽������
    super->MediaInfo = at45_umedia;
    super->MediaDrv = mediadrv;

    if((s32)dwEnd == -1)
    {
        dwEnd = nordescription->Blks;
        BlockNum = dwEnd - dwStart;     //��ȡ�ļ�ϵͳһ�����˶��ٿ�
    }
    else
    {
        BlockNum = dwEnd - dwStart;
    }
    super->AreaSize = BlockNum * (nordescription->BytesPerPage * nordescription->SectorsPerBlk);
    super->MediaStart = dwStart * nordescription->SectorsPerBlk; // ��ʼunit��

    res = strlen(At45Name) + strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,At45Name);      //��ȡ�豸��ȫ·��
    File_BeMedia(FullPath,fs);     //�����豸�����ļ�ϵͳ
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, dwStart, dwEnd);
    return (0);
}
#endif
