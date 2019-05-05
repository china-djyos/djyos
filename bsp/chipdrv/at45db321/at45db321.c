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
#include <device/flash/flash.h>
#include "os.h"
#include "systime.h"
#include <math.h>
#include <stdlib.h>
#include <device/include/unit_media.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_at45db321(char *pBusName, const char *TargetFs, s32 bstart, s32 bend, u32 doformat);
//    ModuleInstall_at45db321(CFG_AT45_BUSNAME, CFG_AT45_FSMOUNT_NAME, CFG_AT45_PART_START, CFG_AT45_PART_END, CFG_AT45_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"at45db321"    //spi�ӿڵ�norflash
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","spibus","heap",,"cpu_peri_spi"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_AT45_BUSNAME           //****�������Ƿ��Ѿ����ú�
#warning   at45db321�������δ���ã�ʹ��Ĭ��ֵ
//%$#@enum,512,528,
#define CFG_AT45_PAGE_SIZE                  528       //"pagesize",����AT45��ҳ��С��Ĭ��Ϊ528
//%$#@string,1,10,
#define CFG_AT45_BUSNAME                   "SPI4"     //"SPI��������",AT45ʹ�õ���������
#define CFG_AT45_FSMOUNT_NAME              "efs"      //"�ļ�ϵͳmount������",��Ҫ���ص��ļ�ϵͳmount������
//%$#@num,-1,1024,
#define CFG_AT45_PART_START                  0        //������ʼ����д��ţ���Ŵ�0��ʼ����
#define CFG_AT45_PART_END                   -1        //����������-1��ʾ���һ��
//%$#@enum,true,false,
#define CFG_AT45_PART_FORMAT               false      //����ѡ��,�Ƿ���Ҫ��ʽ���÷�����
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
extern s32 __at45_write(s64 unit, void *data, struct uopt opt);
extern s32 __at45_read(s64 unit, void *data, struct uopt opt);
extern s32 __at45_req(enum ucmd cmd, ptu32_t args, ...);
extern s32 __at45_erase(s64 unit, struct uesz sz);
extern s32 __AT45_FsInstallInit(const char *fs, s32 dwStart, s32 dwSize);


bool_t at45db321_Wait_Ready(u32 Time_Out);
/*---------------------test use only----------------------
#define test_buff_size  10240
section("seg_int_data") u32 Data_write[test_buff_size]={0};
section("seg_int_data") u32 Data_read[test_buff_size]={0};
u32 buff1_cnt=0,buff2_cnt=0;
---------------------test use only----------------------*/
void _at45db321_cs_active(void)
{
    SPI_CsActive(s_ptAT45_Dev,AT45_OP_TIMEOUT);
}
void _at45db321_cs_inactive(void)
{
    SPI_CsInactive(s_ptAT45_Dev);
    Djy_DelayUs(20);
}
u32 _at45db321_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen)
{
    struct SPI_DataFrame data;
    s32 result;

    data.RecvBuf = rdata;
    data.RecvLen = rlen;
    data.RecvOff = slen;
    data.SendBuf = sdata;
    data.SendLen = slen;

    result = SPI_Transfer(s_ptAT45_Dev,&data,true,AT45_OP_TIMEOUT);
    if(result != CN_SPI_EXIT_NOERR)
        return 0;
    return 1;
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

    _at45db321_cs_active();

    _at45db321_TxRx(_at45db321_Command,5,data,data_len);

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

    _at45db321_cs_active();

    _at45db321_TxRx(_at45db321_Command,4,NULL,0);
    _at45db321_TxRx(data,data_len,NULL,0);

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

    _at45db321_cs_active();

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
    _at45db321_Command[3] = 0xFF;

    _at45db321_cs_active();

    _at45db321_TxRx(_at45db321_Command,4,NULL,0);

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
    _at45db321_Command[3] = 0xFF;

    _at45db321_cs_active();

    _at45db321_TxRx(_at45db321_Command,4,NULL,0);

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

    _at45db321_cs_active();

    _at45db321_TxRx(_at45db321_Command,4,NULL,0);

    _at45db321_cs_inactive();

    Djy_EventDelay(50000);
    Lock_MutexPost(pAT45_Lock);
    return true;
}

//----SPI FLASH Block����------------------------------------------------------
//���ܣ�SPI FLASH Block����
//��������
//���أ���
//-----------------------------------------------------------------------------
bool_t AT45_Block_Erase(u32 Address)
{
    u32 block_addr;
    if(false == Lock_MutexPend(pAT45_Lock, AT45_OP_TIMEOUT))
    {
        return false;
    }

    if(false == at45db321_Wait_Ready(500000))       //��æ������ʱ�򷵻�false
        return false;//��ʱ���˳�

    block_addr = _at45db321_Page_Caculate(Address);
    block_addr = _at45db321_Block_Caculate(block_addr);

    _at45db321_Command[0] = AT45_Block_Erase_Cmd;
    _at45db321_Command[1] = (block_addr>>4)&0xFF;
    _at45db321_Command[2] = (block_addr<<4)&0xFF;
    _at45db321_Command[3] = 0xFF;

    _at45db321_cs_active();

    _at45db321_TxRx(_at45db321_Command,4,NULL,0);

    _at45db321_cs_inactive();

    Djy_EventDelay(100000);
    Lock_MutexPost(pAT45_Lock);

    return true;
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

    Djy_EventDelay(100000000);
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
        Djy_DelayUs(2);
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
        Djy_EventDelay(1*mS);
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
        return false;//��ʱ���˳�

    _at45db321_Continuous_Array_Read(page_addr,byte_offset_addr,data,data_len);

    Djy_EventDelay(10000);
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
    Djy_EventDelay(10000);
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

#if 1
// =============================================================================
// ���ܣ���װat45����
// ������   pBusName -- AT45��Ҫ�õ�ͨ����
//      TargetFs -- Ҫ���ص��ļ�ϵͳ
//      parts -- ��������
//      TargetPart -- ָ��Ҫ�ҵ��ĸ������£�������0��ʼ
//      �������� -- ��ʼ�飬��������������ʱ�������ÿ飬ֻ�����ÿ����һ�飩���Ƿ��ʽ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// =============================================================================
bool_t ModuleInstall_at45db321(char *pBusName, const char *TargetFs, s32 bstart, s32 bend, u32 doformat)
{
    static u8 at45init = 0;

    if(at45init == 0)
    {
        pAT45_Lock = Lock_MutexCreate("AT45 Lock");
        if(!pAT45_Lock)
        {
            printf("\r\n: error : device : cannot create AT45 flash lock.");
            return false;
        }

        s_ptAT45_Dev = SPI_DevAdd(pBusName,At45Name,0,8,SPI_MODE_0,SPI_SHIFT_MSB,AT45_SPI_SPEED,false);
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
        sAT45Inited = true;
        if(bstart == bend)
        {
            return (0); // ��������
        }

        if(!nordescription) //��ʼ��nor����Ϣ
        {
            nordescription = malloc(sizeof(struct NorDescr));
            if(!nordescription)
            {
                printf("\r\n: erro : device : memory out.\r\n");
                return (-1);
            }

            memset(nordescription, 0x0, (sizeof(struct NorDescr)));

            // AT45��sector��block�󣬶���sector�Ĵ�С��һ�¡������߼��Ͼͽ�sector����page��
            // ��Ȼsector,block���
            nordescription->PortType = NOR_SPI;
            nordescription->Port = s_ptAT45_Dev;
            nordescription->BytesPerPage = 512;
            nordescription->PagesPerSector = 1;
            nordescription->SectorsPerBlk = 8;
            nordescription->Blks = 1024; // ȫ������������
            nordescription->ReservedBlks = 0;
        }

        if(doformat)
        {
            struct uesz sz;
            sz.unit = 0;
            sz.block = 1;
            __at45_req(format, bstart , bend, &sz);
        }

        at45_umedia = malloc(sizeof(struct umedia)+nordescription->BytesPerPage);
        if(!at45_umedia)
            return (-1);

        at45_umedia->mreq = __at45_req;
        at45_umedia->type = nor;
        at45_umedia->ubuf = (u8*)at45_umedia + sizeof(struct umedia);

        if(!dev_Create((const char*)At45Name, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)at45_umedia)))
        {
            printf("\r\n: erro : device : %s addition failed.", At45Name);
            free(at45_umedia);
            return (-1);
        }
        at45init = 1;
    }

    if(TargetFs != NULL)
    {
        if(__AT45_FsInstallInit(TargetFs, bstart, bend))
        {
            return -1;
        }
    }

    return sAT45Inited;
}
#else
// =============================================================================
// ���ܣ���ʼ��SPI FLASHģ�飬У��оƬID�Ƿ���ȷ
// ������bArgC -- ����������
//      ����1 -- SPI�˿ڣ����룩��
//      ����2 -- ����1��ʼ�飻
//      ����3 -- ����1��С����Ϊ��λ����
//      ����4 -- ����1��;������EFS��2�������������ļ�ϵͳ��1����
//      ����n -- �ظ����������߼���
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע�������߼������ļ�ϵͳ��ֱ�ӷ����߼��������÷�����
// =============================================================================
s32 ModuleInstall_AT45DB321E(u8 bArgC, ...)
{
    va_list ap;
    char *bus = NULL;
    u32 start[3] = {0};
    u32 size[3] = {0};
    u32 special[3] = {0};
    u8 partitions = 0, i;
    static char *name = "AT45DB321E";
    extern s32 __AT45_MakePartition(char *pName, u32 dwStart, u32 dwSize, u32 dwSpecial, void *pPrivate);

    va_start(ap, bArgC);
    for(i = 0; i < bArgC; i++)
    {
        switch(i)
        {
            case 0 : bus = (char*)va_arg(ap, u32); break;
            case 1 : start[0] = va_arg(ap, u32); break;
            case 2 : size[0] = va_arg(ap, u32); ; break;
            case 3 : special[0] = va_arg(ap, u32); partitions++;break;
            case 4 : start[1] = va_arg(ap, u32); break;
            case 5 : size[1] = va_arg(ap, u32); break;
            case 6 : special[1] = va_arg(ap, u32); partitions++;break;
            case 7 : start[2] = va_arg(ap, u32); break;
            case 8 : size[2] = va_arg(ap, u32); ;break;
            case 9 : special[2] = va_arg(ap, u32); partitions++;break;
            default: break;
        }
    }
    va_end(ap);

    if(((size[0]+size[1]+size[2]) > 1024) || (!bus))
    {
        printf("\r\n: error : device : bad parameters for install flash <at45db321e>.");
        return (-1);
    }

    pAT45_Lock = Lock_MutexCreate("AT45 Lock");
    if(!pAT45_Lock)
    {
        printf("\r\n: error : device : cannot create AT45 flash lock.");
        return (-1);
    }

    s_ptAT45_Dev = SPI_DevAdd(bus,name,0,8,SPI_MODE_0,SPI_SHIFT_MSB,AT45_SPI_SPEED,false);
    if(s_ptAT45_Dev != NULL)
    {
        SPI_BusCtrl(s_ptAT45_Dev, CN_SPI_SET_POLL, 0, 0);
    }

    if(FALSE == _at45db321_Check_ID())  //У��оƬID
    {
        printf("\r\n: error : device : bad ID check for <at45db321e>.");
        return (-1);
    }

    if(!(_at45db321_Read_Status() & AT45_Status_Reg_Bit_PGSZ)) //ת����512�ֽ�
        _at45db321_Binary_Page_Size_512();

    sAT45Inited = TRUE;

    for(i = 0; i < partitions; i++)
        __AT45_MakePartition(name, start[i], size[i], special[i], s_ptAT45_Dev);

    return sAT45Inited;
}
#endif

