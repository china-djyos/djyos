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
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
#include <Iboot_info.h>
#include "stdint.h"
#include "stddef.h"
#include "board-config.h"
#include "component_config_iboot.h"
//-----------------------------------------------------------------------------
//����ģ��: si ibootģ��
//����:  czz
//�汾��V1.0.0
//�ļ�����:ʵ��APP��Ӧ��bin�ļ���ʽ
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "shell.h"
extern void AppStart(void);
extern void Init_Cpu(void);
extern void reboot();
extern void Load_Preload(void);
extern void * gc_pAppOffset;
extern void __asm_bl_fun(void * fun_addr);
//�汾��
#define APP_HEAD_VERSION        1
#if defined(DEBUG)
#define BUILD_IS_DEBUG     (1)//��ǰAPP�汾debug
#else
#define BUILD_IS_DEBUG     (0)//��ǰAPP�汾release
#endif

struct AppHead
{
    char djyflag[3];        //"djy"��־                    �̶���־
    u8   AppVer;            //��Ϣ��İ汾
    u32  filesize;          //�ļ�ϵͳ�������ļ���С   ��������ʱ ���ļ�ϵͳ������ʱ���ⲿ�������

    #define VERIFICATION_NULL     0
    #define VERIFICATION_CRC      1
    #define VERIFICATION_MD5      2
    #define VERIFICATION_SSL      3  //SSL��ȫ֤��
    u32  Verification;    //У�鷽��Ĭ��У�鷽��Ϊ��У�飬���ⲿ���߸��������޸�
    u32  appbinsize;      //app bin�ļ���С ���ⲿ�������
    u32  VirtAddr;        //���е�ַ
    u32  appheadsize;     //��Ϣ��Ĵ�С
    u32  isdebug;          //�Ƿ�Ϊdebug�汾
    u32  reserved;          //����
    char appname[96];      //app���ļ��� ���ⲿ��������bin�ļ����ļ���
    char VerifBuf[128];     //У������У�鷽����Ӧ�ľ������� �ɹ������
}const Djy_App_Head __attribute__ ((section(".DjyAppHead"))) =
{
        .djyflag[0]    = 'd',
        .djyflag[1]    = 'j',
        .djyflag[2]    = 'y',
        .AppVer        = APP_HEAD_VERSION,
        .filesize      = 0xffffffff,
        .Verification  = CFG_APP_VERIFICATION,
        .appbinsize    = 0xffffffff,
        .VirtAddr      = (u32)&Djy_App_Head,
        .appheadsize   = sizeof(struct AppHead),
        .isdebug       = BUILD_IS_DEBUG,
        .reserved      = 0xffffffff,
        .appname       = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff},

        .VerifBuf      = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}
};


#define IBOOT_APP_INFO_VER                 (1)
struct IbootAppInfo
{
    #define PREVIOURESET_IBOOT   (0x12345678)//��λǰ����iboot
    #define PREVIOURESET_APP     (0x87654321)//��λǰ����APP
    u32 PreviouReset;//��λǰ����ģʽ
    struct{
        u32 heard_set_run_iboot   :1;//Ӳ����������iboot
        u32 restart_run_iboot     :1;//ָʾ����������Iboot
        u32 restart_run_app       :1;//ָʾ����������APP
        u32 runmode_iboot         :1;//��ǰ����ģʽ��iboot
        u32 runmode_app           :1;//��ǰ����ģʽΪapp
        u32 Before_run_iboot      :1;//֮ǰ����ģʽΪiboot
        u32 Before_run_app        :1;//֮ǰ����ģʽΪapp
        u32 run_app_form_file     :1;//���ļ��м���app
        u32 run_iboot_update_app  :1;//������Iboot�����Զ�����APP
        u32 run_iboot_update_iboot:1;//��iboot������iboot����
        u32 update_from           :2;//�����ļ���Դ0�ļ� 1����3������
        u32 update_runmode        :1;//������ɺ�����0.iboot --  1.app
        u32 error_app_check       :1;//У�����
        u32 error_app_no_file     :1;//û������ļ����ļ���ʽ����
        u32 error_app_size        :1;//app�ļ���С����
        //�ϵ縴λӲ����־0=�޴�Ӳ�� 1=�д�Ӳ�������ޱ�־��2=�б�־���ĺ󼴷٣�3=�У����ĺ󼴷٣�
        u32 power_on_flag         :2;
        u32 head_wdt_reset        :1;//���Ź���λ��־
        u32 soft_reset_flag       :1;//����������ڲ���λ
        u32 reboot_flag           :1;//reboot ��־
        u32 head_reset_flag       :1;//�ⲿӲ����λ��־
        u32 instructions_reset    :1;//ָ������ĸ�λ
        u32 low_power_wakeup      :1;//�͹�����������жϻ��ѱ�־
        u32 call_fun_resent       :1;//1=�ڲ���λ/����������������غ��������ģ�0=�쳣����
        u32 power_on_resent_flag  :1;//�ϵ縴λ��־�����b18~19�Լ����ϵ��־�����ж�
    }runflag; //���б�־
    u32 reserved;//����
    u16  buildyear;    /* years since 1900 */
    u8   buildmon;     /* months since January [0-11] */
    u8   buildmday;    /* day of the month [1-31] */
    u8   buildhour;    /* hours since midnight [0-23] */
    u8   buildmin;     /* minutes after the hour [0-59] */
    u8   buildsec;     /* seconds after the minute [0-60] */
    u8   ibootVer;         //iboot �汾
    u64  ibootstartaddr;   //iboot������ַ
    u8   ibootisdebug;
    char boardname[23];    //�����
    u8   rsvbuf[12];  //����
}Iboot_App_Info __attribute__ ((section(".IbootAppInfo"))) ;


//==============================================================================
//���ܣ���ȡӲ���ϵı�־
//������POWER_ON_FLAG����ȡ�ϵ縴λӲ����־��0=�޴�Ӳ����1=�д�Ӳ����
//                  ���ޱ�־��2=�б�־���ĺ󼴷٣�3=�У����ĺ󼴷�
//     HEAD_RESET_FLAG�� ��ȡӲ����λ��־û��/��֧�ַ���0
//     HEAD_WDT_RESE�� ��ȡӲ�����Ź���λ��־û��/��֧�ַ���0
//     LOW_POWER_WAKEUP�� �͹��Ļ���û��/��֧�ַ���0
//==============================================================================
__attribute__((weak))  u8  Get_Headflag(enum headflag flag)
{
    switch (flag)
    {
       case POWER_ON_FLAG  :  break;
       case HEAD_RESET_FLAG:  break;
       case HEAD_WDT_RESET :  break;
       case LOW_POWER_WAKEUP: break;
       default:               break;
    }
    return 0;
}


//==============================================================================
//���APP�İ汾��debug
//
//==============================================================================
bool_t Chack_AppTypeIsDebug(void * apphead)
{
    struct AppHead*  p_apphead = apphead;

    if(p_apphead->Verification&1)
        return true;
    return false;
}
//=============================================================================
//=============================================================================
#if (CFG_APP_VERIFICATION == VERIFICATION_CRC)
// CRC32��ѯ��
static const u32 crc32_tab[] = {     // CRC polynomial 0xedb88320
0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

// ============================================================================
// ���ܣ�CRC32�ֲ������ʼ��
// ������crc -- CRCֵ
// ���أ�0 -- ʧ�ܣ�-1 -- ʧ�ܣ�
// ��ע��
// ============================================================================
static s32 iboot_Crc32init(u32 *crc)
{
    if(!crc)
        return (0);
    *crc = -1;
    return (-1);
}

// ============================================================================
// ���ܣ�CRC32�ֲ�����
// ������crc -- CRCֵ��buf -- ���ݣ�len -- ���ݳ��ȣ�
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 iboot_Crc32run(u32 *crc, u8 *buf, u32 len)
{
    u32 i;
    u32 tmp;

    if((!crc) || (!buf))
        return (-1);

    tmp = *crc;
    for(i = 0; i < len;i++)
        tmp = crc32_tab[(tmp ^ buf[i]) & 0xFF] ^ (tmp >> 8);

    *crc = tmp;

    return (0);
}

// ============================================================================
// ���ܣ�CRC32�ֲ������˳�
// ������crc -- CRCֵ��
// ���أ�0 -- ʧ�ܣ�-1 -- ʧ�ܣ�
// ��ע��
// ============================================================================
static s32 iboot_Crc32exit(u32 *crc)
{
    u32 tmp;

    if(!crc)
        return (0);

    tmp = *crc;
    tmp = ~tmp;
    *crc = tmp;

    return (-1);
}
#elif( CFG_APP_VERIFICATION  == VERIFICATION_MD5 )
//define for the data types
typedef unsigned int u32_t;

/* Data structure for MD5 (Message-Digest) computation */
typedef struct {
  u32_t i[2];               /* number of _bits_ handled mod 2^64 */
  u32_t buf[4];             /* scratch buffer */
  unsigned char in[64];     /* input buffer */
  unsigned char digest[16]; /* actual digest after MD5Final call */
} MD5_CTX;

/*
 ***********************************************************************
 **  Message-digest routines:                                         **
 **  To form the message digest for a message M                       **
 **    (1) Initialize a context buffer mdContext using MD5Init        **
 **    (2) Call MD5Update on mdContext and M                          **
 **    (3) Call MD5Final on mdContext                                 **
 **  The message digest is now in mdContext->digest[0...15]           **
 ***********************************************************************
 */

/* forward declaration */
static void Transform (u32_t *buf, u32_t *in);

static unsigned char PADDING[64] = {
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* F, G, H and I are basic MD5 functions */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4 */
/* Rotation is separate from addition to prevent recomputation */
#define FF(a, b, c, d, x, s, ac) \
  {(a) += F ((b), (c), (d)) + (x) + (u32_t)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
  }
#define GG(a, b, c, d, x, s, ac) \
  {(a) += G ((b), (c), (d)) + (x) + (u32_t)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
  }
#define HH(a, b, c, d, x, s, ac) \
  {(a) += H ((b), (c), (d)) + (x) + (u32_t)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
  }
#define II(a, b, c, d, x, s, ac) \
  {(a) += I ((b), (c), (d)) + (x) + (u32_t)(ac); \
   (a) = ROTATE_LEFT ((a), (s)); \
   (a) += (b); \
  }

#ifdef __STDC__
#define UL(x) x##UL
#else
#ifdef WIN32
#define UL(x) x##UL
#else
#define UL(x) x
#endif
#endif

/* The routine MD5Init initializes the message-digest context
   mdContext. All fields are set to zero.
 */
static void MD5Init (MD5_CTX *mdContext)
{
  mdContext->i[0] = mdContext->i[1] = (u32_t)0;

  /* Load magic initialization constants. */
  mdContext->buf[0] = (u32_t)0x67452301UL;
  mdContext->buf[1] = (u32_t)0xefcdab89UL;
  mdContext->buf[2] = (u32_t)0x98badcfeUL;
  mdContext->buf[3] = (u32_t)0x10325476UL;
}

/* The routine MD5Update updates the message-digest context to
   account for the presence of each of the characters inBuf[0..inLen-1]
   in the message whose digest is being computed.
 */
static void MD5Update(MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen)
{
  u32_t in[16];
  int mdi;
  unsigned int i, ii;

#if 0
  PPPDEBUG(LOG_INFO, ("MD5Update: %u:%.*H\n", inLen, LWIP_MIN(inLen, 20) * 2, inBuf));
  PPPDEBUG(LOG_INFO, ("MD5Update: %u:%s\n", inLen, inBuf));
#endif

  /* compute number of bytes mod 64 */
  mdi = (int)((mdContext->i[0] >> 3) & 0x3F);

  /* update number of bits */
  if ((mdContext->i[0] + ((u32_t)inLen << 3)) < mdContext->i[0]) {
    mdContext->i[1]++;
  }
  mdContext->i[0] += ((u32_t)inLen << 3);
  mdContext->i[1] += ((u32_t)inLen >> 29);

  while (inLen--) {
    /* add new character to buffer, increment mdi */
    mdContext->in[mdi++] = *inBuf++;

    /* transform if necessary */
    if (mdi == 0x40) {
      for (i = 0, ii = 0; i < 16; i++, ii += 4) {
        in[i] = (((u32_t)mdContext->in[ii+3]) << 24) |
                (((u32_t)mdContext->in[ii+2]) << 16) |
                (((u32_t)mdContext->in[ii+1]) << 8)  |
                ((u32_t)mdContext->in[ii]);
      }
      Transform (mdContext->buf, in);
      mdi = 0;
    }
  }
}

/* The routine MD5Final terminates the message-digest computation and
   ends with the desired message digest in mdContext->digest[0...15].
 */
static void MD5Final (unsigned char hash[], MD5_CTX *mdContext)
{
  u32_t in[16];
  int mdi;
  unsigned int i, ii;
  unsigned int padLen;

  /* save number of bits */
  in[14] = mdContext->i[0];
  in[15] = mdContext->i[1];

  /* compute number of bytes mod 64 */
  mdi = (int)((mdContext->i[0] >> 3) & 0x3F);

  /* pad out to 56 mod 64 */
  padLen = (mdi < 56) ? (56 - mdi) : (120 - mdi);
  MD5Update (mdContext, PADDING, padLen);

  /* append length in bits and transform */
  for (i = 0, ii = 0; i < 14; i++, ii += 4) {
    in[i] = (((u32_t)mdContext->in[ii+3]) << 24) |
            (((u32_t)mdContext->in[ii+2]) << 16) |
            (((u32_t)mdContext->in[ii+1]) << 8)  |
            ((u32_t)mdContext->in[ii]);
  }
  Transform (mdContext->buf, in);

  /* store buffer in digest */
  for (i = 0, ii = 0; i < 4; i++, ii += 4) {
    mdContext->digest[ii]   = (unsigned char)(mdContext->buf[i] & 0xFF);
    mdContext->digest[ii+1] =
      (unsigned char)((mdContext->buf[i] >> 8)  & 0xFF);
    mdContext->digest[ii+2] =
      (unsigned char)((mdContext->buf[i] >> 16) & 0xFF);
    mdContext->digest[ii+3] =
      (unsigned char)((mdContext->buf[i] >> 24) & 0xFF);
  }
  memcpy(hash, mdContext->digest, 16);
}

/* Basic MD5 step. Transforms buf based on in.
 */
static void Transform (u32_t *buf, u32_t *in)
{
  u32_t a = buf[0], b = buf[1], c = buf[2], d = buf[3];

  /* Round 1 */
#define S11 7
#define S12 12
#define S13 17
#define S14 22
  FF ( a, b, c, d, in[ 0], S11, UL(3614090360)); /* 1 */
  FF ( d, a, b, c, in[ 1], S12, UL(3905402710)); /* 2 */
  FF ( c, d, a, b, in[ 2], S13, UL( 606105819)); /* 3 */
  FF ( b, c, d, a, in[ 3], S14, UL(3250441966)); /* 4 */
  FF ( a, b, c, d, in[ 4], S11, UL(4118548399)); /* 5 */
  FF ( d, a, b, c, in[ 5], S12, UL(1200080426)); /* 6 */
  FF ( c, d, a, b, in[ 6], S13, UL(2821735955)); /* 7 */
  FF ( b, c, d, a, in[ 7], S14, UL(4249261313)); /* 8 */
  FF ( a, b, c, d, in[ 8], S11, UL(1770035416)); /* 9 */
  FF ( d, a, b, c, in[ 9], S12, UL(2336552879)); /* 10 */
  FF ( c, d, a, b, in[10], S13, UL(4294925233)); /* 11 */
  FF ( b, c, d, a, in[11], S14, UL(2304563134)); /* 12 */
  FF ( a, b, c, d, in[12], S11, UL(1804603682)); /* 13 */
  FF ( d, a, b, c, in[13], S12, UL(4254626195)); /* 14 */
  FF ( c, d, a, b, in[14], S13, UL(2792965006)); /* 15 */
  FF ( b, c, d, a, in[15], S14, UL(1236535329)); /* 16 */

  /* Round 2 */
#define S21 5
#define S22 9
#define S23 14
#define S24 20
  GG ( a, b, c, d, in[ 1], S21, UL(4129170786)); /* 17 */
  GG ( d, a, b, c, in[ 6], S22, UL(3225465664)); /* 18 */
  GG ( c, d, a, b, in[11], S23, UL( 643717713)); /* 19 */
  GG ( b, c, d, a, in[ 0], S24, UL(3921069994)); /* 20 */
  GG ( a, b, c, d, in[ 5], S21, UL(3593408605)); /* 21 */
  GG ( d, a, b, c, in[10], S22, UL(  38016083)); /* 22 */
  GG ( c, d, a, b, in[15], S23, UL(3634488961)); /* 23 */
  GG ( b, c, d, a, in[ 4], S24, UL(3889429448)); /* 24 */
  GG ( a, b, c, d, in[ 9], S21, UL( 568446438)); /* 25 */
  GG ( d, a, b, c, in[14], S22, UL(3275163606)); /* 26 */
  GG ( c, d, a, b, in[ 3], S23, UL(4107603335)); /* 27 */
  GG ( b, c, d, a, in[ 8], S24, UL(1163531501)); /* 28 */
  GG ( a, b, c, d, in[13], S21, UL(2850285829)); /* 29 */
  GG ( d, a, b, c, in[ 2], S22, UL(4243563512)); /* 30 */
  GG ( c, d, a, b, in[ 7], S23, UL(1735328473)); /* 31 */
  GG ( b, c, d, a, in[12], S24, UL(2368359562)); /* 32 */

  /* Round 3 */
#define S31 4
#define S32 11
#define S33 16
#define S34 23
  HH ( a, b, c, d, in[ 5], S31, UL(4294588738)); /* 33 */
  HH ( d, a, b, c, in[ 8], S32, UL(2272392833)); /* 34 */
  HH ( c, d, a, b, in[11], S33, UL(1839030562)); /* 35 */
  HH ( b, c, d, a, in[14], S34, UL(4259657740)); /* 36 */
  HH ( a, b, c, d, in[ 1], S31, UL(2763975236)); /* 37 */
  HH ( d, a, b, c, in[ 4], S32, UL(1272893353)); /* 38 */
  HH ( c, d, a, b, in[ 7], S33, UL(4139469664)); /* 39 */
  HH ( b, c, d, a, in[10], S34, UL(3200236656)); /* 40 */
  HH ( a, b, c, d, in[13], S31, UL( 681279174)); /* 41 */
  HH ( d, a, b, c, in[ 0], S32, UL(3936430074)); /* 42 */
  HH ( c, d, a, b, in[ 3], S33, UL(3572445317)); /* 43 */
  HH ( b, c, d, a, in[ 6], S34, UL(  76029189)); /* 44 */
  HH ( a, b, c, d, in[ 9], S31, UL(3654602809)); /* 45 */
  HH ( d, a, b, c, in[12], S32, UL(3873151461)); /* 46 */
  HH ( c, d, a, b, in[15], S33, UL( 530742520)); /* 47 */
  HH ( b, c, d, a, in[ 2], S34, UL(3299628645)); /* 48 */

  /* Round 4 */
#define S41 6
#define S42 10
#define S43 15
#define S44 21
  II ( a, b, c, d, in[ 0], S41, UL(4096336452)); /* 49 */
  II ( d, a, b, c, in[ 7], S42, UL(1126891415)); /* 50 */
  II ( c, d, a, b, in[14], S43, UL(2878612391)); /* 51 */
  II ( b, c, d, a, in[ 5], S44, UL(4237533241)); /* 52 */
  II ( a, b, c, d, in[12], S41, UL(1700485571)); /* 53 */
  II ( d, a, b, c, in[ 3], S42, UL(2399980690)); /* 54 */
  II ( c, d, a, b, in[10], S43, UL(4293915773)); /* 55 */
  II ( b, c, d, a, in[ 1], S44, UL(2240044497)); /* 56 */
  II ( a, b, c, d, in[ 8], S41, UL(1873313359)); /* 57 */
  II ( d, a, b, c, in[15], S42, UL(4264355552)); /* 58 */
  II ( c, d, a, b, in[ 6], S43, UL(2734768916)); /* 59 */
  II ( b, c, d, a, in[13], S44, UL(1309151649)); /* 60 */
  II ( a, b, c, d, in[ 4], S41, UL(4149444226)); /* 61 */
  II ( d, a, b, c, in[11], S42, UL(3174756917)); /* 62 */
  II ( c, d, a, b, in[ 2], S43, UL( 718787259)); /* 63 */
  II ( b, c, d, a, in[ 9], S44, UL(3951481745)); /* 64 */

  buf[0] += a;
  buf[1] += b;
  buf[2] += c;
  buf[3] += d;
}

#elif( CFG_APP_VERIFICATION  == VERIFICATION_SSL )

#elif (CFG_APP_VERIFICATION  == VERIFICATION_NULL)

#endif


static bool_t Verification_AppExit(void * apphead)
{
    struct AppHead*  p_apphead = apphead;
#if (CFG_APP_VERIFICATION == VERIFICATION_CRC)
    iboot_Crc32exit((u32*)p_apphead->VerifBuf);
#elif( CFG_APP_VERIFICATION  == VERIFICATION_MD5 )
    unsigned char hash[16];
    u32 i;

    MD5Final(hash, (MD5_CTX*)p_apphead->VerifBuf);
    for(i =0;i<sizeof(MD5_CTX);i++)
        p_apphead->VerifBuf[i]=0xff;
    for(i =0;i<16;i++)
        p_apphead->VerifBuf[i]=hash[i];
#elif( CFG_APP_VERIFICATION  == VERIFICATION_SSL )

#elif (CFG_APP_VERIFICATION  == VERIFICATION_NULL)

#endif

    return true;
}




static bool_t Verification_AppRun(void * apphead, u8 * buf, u32 len)
{
    struct AppHead*  p_apphead = apphead;
#if (CFG_APP_VERIFICATION == VERIFICATION_CRC)
    iboot_Crc32run((u32*)p_apphead->VerifBuf,buf,len);
#elif( CFG_APP_VERIFICATION  == VERIFICATION_MD5 )
    MD5Update((MD5_CTX*)p_apphead->VerifBuf, buf,len);
#elif( CFG_APP_VERIFICATION  == VERIFICATION_SSL )

#elif (CFG_APP_VERIFICATION  == VERIFICATION_NULL)

#endif
    return true;
}

static bool_t Verification_AppInit(void * apphead)
{
    struct AppHead*  p_apphead = apphead;
    u32 i;

    p_apphead->filesize = 0xFFFFFFFF;          //�ļ�ϵͳ�������ļ���С   ��������ʱ ���ļ�ϵͳ������ʱ���ⲿ�������
    for( i=0;i<sizeof(p_apphead->appname);i++)
        p_apphead->appname[i]=0xff;

    for(u32 i =0;i<sizeof(p_apphead->VerifBuf);i++)
        p_apphead->VerifBuf[i]=0xff;

#if (CFG_APP_VERIFICATION == VERIFICATION_CRC)
    iboot_Crc32init((u32*)p_apphead->VerifBuf);
    iboot_Crc32run((u32*)p_apphead->VerifBuf,apphead,Get_AppHeadSize());
#elif( CFG_APP_VERIFICATION  == VERIFICATION_MD5 )
    MD5_CTX md5tmp;
    char *buf = (char *)&md5tmp;
    MD5Init(&md5tmp);
    MD5Update(&md5tmp, apphead, p_apphead->appheadsize);
    for(u32 i =0;i< sizeof(MD5_CTX);i++)
        p_apphead->VerifBuf[i]=buf[i];
#elif( CFG_APP_VERIFICATION  == VERIFICATION_SSL )

#elif (CFG_APP_VERIFICATION  == VERIFICATION_NULL)

#endif


    return true;
}
static bool_t  Verification_compare(void *apphead,void *appheadcmp)
{
    struct AppHead*  p_apphead = apphead;
    struct AppHead*  p_appheadcmp = appheadcmp;
    u32 i;
    for(i=0;i<sizeof(p_apphead->VerifBuf);i++)
    {
        if(p_apphead->VerifBuf[i]!=p_appheadcmp->VerifBuf[i]&&p_apphead->VerifBuf[i]!=0xFF)
            return false;
    }
    return true;
}

bool_t Rewrite_AppHead(void * apphead,const char*name,u32 filesize)
{
    struct AppHead*  p_apphead = apphead;
    u8 flag = 1;
    for(u8 i=0;i<sizeof(p_apphead->appname);i++)
    {
        if(flag)
            p_apphead->appname[i] = name[i];
        else
            p_apphead->appname[i] = 0xff;
        if(name[i]==0)
            flag=0;
    }
    p_apphead->filesize = filesize;
    if(flag)
        return false;
    return true;
}

//==============================================================================
//���ܣ�Directģʽ����APP
//������
//���أ�
//==============================================================================
static bool_t __RunApp(void * apphead)
{
    struct AppHead*  p_apphead = apphead;
    struct AppHead app_head;

    u8 * buf = (u8*)&app_head;
    u8 *bufapp = apphead;
    u32 i;
    if(p_apphead->djyflag[0]!='d' ||p_apphead->djyflag[1]!='j' ||p_apphead->djyflag[2]!='y' )
    {
        Iboot_App_Info.runflag.error_app_no_file = 1;
        return false;
    }
    for(i=0;i<sizeof(struct AppHead);i++)
        buf[i] = bufapp[i];
    Iboot_App_Info.runflag.restart_run_app = 0;
    Iboot_App_Info.runflag.error_app_check = 0;
    Iboot_App_Info.runflag.error_app_size  = 0;
    if(p_apphead->appbinsize != p_apphead->filesize)
    {
        Iboot_App_Info.runflag.error_app_size  = 1;
    }
    if(p_apphead->Verification !=VERIFICATION_NULL)
    {
        Verification_AppInit(&app_head);
        Verification_AppRun(&app_head,apphead+sizeof(struct AppHead),p_apphead->appbinsize-sizeof(struct AppHead));
        Verification_AppExit(&app_head);
        if(false == Verification_compare(apphead,&app_head))
        {
            Iboot_App_Info.runflag.error_app_check = 1;
            return false;
        }
    }
    Iboot_App_Info.runflag.runmode_app     = 1;
    __asm_bl_fun((u32)p_apphead+sizeof(struct AppHead));
    return false;
}

//==============================================================================
//����:����APP
//
//
//
//
//==============================================================================
bool_t Run_App(enum runappmode mode)
{
    Iboot_App_Info.runflag.heard_set_run_iboot  = 0;
    Iboot_App_Info.runflag.reboot_flag          = 0;
    switch (mode)
    {
        case RUN_APP_FROM_FILE   :
            Iboot_App_Info.PreviouReset = PREVIOURESET_IBOOT;
            Iboot_App_Info.runflag.runmode_app         = 1;
            Load_Preload();
            return false;
        case RUN_APP_FROM_DIRECT :
            Iboot_App_Info.PreviouReset = PREVIOURESET_APP;
            Iboot_App_Info.runflag.runmode_iboot         = 0;
            Iboot_App_Info.runflag.restart_run_app       = 0;
            __RunApp(gc_pAppOffset);
            return false;
        default:          break;
    }
    return false;
}

//==============================================================================
//���ܣ���ȡAPP�ļ�ͷ��Ϣ�Ĵ�С
//��������
//���أ�APP��Ϣͷ�Ĵ�С��
//==============================================================================
u32 Get_AppHeadSize(void)
{
   return (u32)sizeof(struct AppHead);
}

//==============================================================================
//App ͷ�ļ��������
//
//==============================================================================
bool_t XIP_AppFileChack(void * apphead)
{
    struct AppHead*  p_apphead = apphead;
    if(p_apphead->djyflag[0]!='d' || \
       p_apphead->djyflag[1]!='j' || \
       p_apphead->djyflag[2]!='y' ||\
       p_apphead->appheadsize !=Get_AppHeadSize())
    {
        return false;
    }

    return true;
}

//==============================================================================
//���ܣ���ȡAPP�ļ���С
//��������
//���أ�APP��Ϣͷ�Ĵ�С��
//==============================================================================
u32 XIP_GetAPPStartAddr(void * apphead)
{
    struct AppHead*  p_apphead = apphead;
    return  p_apphead->VirtAddr;
}

u32 XIP_GetAPPSize(void * apphead)
{
    struct AppHead*  p_apphead = apphead;
    return  p_apphead->filesize;
}

bool_t XIP_APPIsDebug(void * apphead)
{
    struct AppHead*  p_apphead = apphead;
    if(p_apphead->isdebug)
        return true;
    return false;
}

u32 Get_AppSize(void * apphead)
{
    struct AppHead*  p_apphead = apphead;
    return  p_apphead->appbinsize;
}
//��ȡAPP����
char* Get_AppName(void * apphead)
{
    struct AppHead*  p_apphead = apphead;
    return  p_apphead->appname;
}



//����APP
bool_t File_Run_App(void * apphead)
{
    struct AppHead*  p_apphead = apphead;
    Iboot_App_Info.runflag.runmode_iboot= 0;
    Iboot_App_Info.runflag.runmode_app  = 1;
    Iboot_App_Info.runflag.restart_run_app = 0;

    Iboot_App_Info.runflag.error_app_no_file = 0;
    Iboot_App_Info.runflag.error_app_check = 0;
    Iboot_App_Info.runflag.error_app_size = 0;

    Iboot_App_Info.runflag.run_iboot_update_app = 0;

    __asm_bl_fun((void*)p_apphead+sizeof(struct AppHead));
    return false;
}

//==============================================================================
//���ܣ��ж��Ƿ�Ϊram�б������iboot
//������
//���أ� true/false
//==============================================================================
bool_t XIP_IsRamIbootFlag()
{
    if(Iboot_App_Info.runflag.restart_run_iboot ==1)
        return true;
    return false;
}

//==============================================================================
//���ܣ���ȡ����ʱ��
//������������ʱ����Ĵ�ŵ�ַ
//���أ� true/false
//��ע����ΪҪ�ŵ�������������Ծ��������ⲿ�ļ��ĺ���ʵ��
//==============================================================================
static bool_t Get_Bulid_Time(u16 *pyear,u8 *pmon,u8 *pday,u8 *phour,u8 *pmin,u8 *psec)
{

    char *time = __TIME__;/*11:35:09*/
    char *date = __DATE__;/*Dec 27 2018*/
    char *p = date;
    *phour = 10*(time[0]-0x30)+(time[1]-0x30);
    *pmin = 10*(time[3]-0x30)+(time[4]-0x30);
    *psec = 10*(time[6]-0x30)+(time[7]-0x30);

    u8 num = 20;
    while(num--)//�ݴ�
    {
        if(*p==0)
            return false;
        if(*p == ' ')
            break;
        p++;
    }
    *pday =  10*(p[1]-0x30)+(p[2]-0x30);
    p+=3;
    while(num--)//�ݴ�
    {
        if(*p==0)
            return false;
        if(*p == ' ')
            break;
        p++;
    }
    *pyear = 1000*(p[1]-0x30)+100*(p[2]-0x30)+10*(p[3]-0x30)+(p[4]-0x30);
    if(date[0]=='J'&&date[1]=='a'&&date[2]=='n')//Jan
        *pmon = 1;
    else if(date[0]=='F'&&date[1]=='e'&&date[2]=='b')//Feb

        *pmon = 2;
    else if(date[0]=='M'&&date[1]=='a'&&date[2]=='r')//Mar
        *pmon = 3;
    else if(date[0]=='A'&&date[1]=='p'&&date[2]=='r')//Apr
        *pmon = 4;
    else if(date[0]=='M'&&date[1]=='a'&&date[2]=='y')//May
        *pmon = 5;
    else if(date[0]=='J'&&date[1]=='u'&&date[2]=='n'&&date[3]=='e')//June
        *pmon = 6;
    else if(date[0]=='J'&&date[1]=='u'&&date[2]=='l'&&date[3]=='y')//July
        *pmon = 7;
    else if(date[0]=='A'&&date[1]=='u'&&date[2]=='g')//Aug
        *pmon = 8;
    else if(date[0]=='S'&&date[1]=='e'&&date[2]=='p'&&date[2]=='t')//Sept
        *pmon = 9;
    else if(date[0]=='O'&&date[1]=='c'&&date[2]=='t')//Oct
        *pmon = 10;
    else if(date[0]=='N'&&date[1]=='o'&&date[2]=='v')//Nov
        *pmon = 11;
    else if(date[0]=='D'&&date[1]=='e'&&date[2]=='c')//Dec
        *pmon = 12;
    else
        return false;
    if(num==0)
        return false;
    return true;
}
//==============================================================================
//���ܣ����������������Ϣ
//������boardname ����� �����ĵ�ַ �����󳤶Ȱ���������
//���أ� true/false
//==============================================================================
static bool_t Fill_boardname(char* boardname,char* buf,u8 maxlen)
{
    u8 i;
    for(i=0;i<maxlen;i++)
    {
        if(*boardname != 0)
            *buf++ = *boardname++;
        else
        {
            *buf++ = *boardname++;
            break;
        }
    }
    if(i==maxlen)
    {
        buf[maxlen-1] = 0;
        return false;
    }
    return true;


}
/*==============================================================================
 * ����    ��siģʽ��ʼ��iboot��APP�����ڴ���Ϣ
 * ����    ��NULL
 * ����ֵ��true/false
==============================================================================*/
bool_t Si_IbootAppInfoInit()
{
    bool_t initflag = false;
    u8 headflag = Get_Headflag(POWER_ON_FLAG);
    switch (headflag)
    {
        case 0: //0=�޴�Ӳ��
            if((Iboot_App_Info.PreviouReset != PREVIOURESET_APP) && \
               (Iboot_App_Info.PreviouReset != PREVIOURESET_IBOOT))
                initflag = true;
            break;
        case 1: break; //1=�д�Ӳ�������ޱ�־��
        case 2: break; //2=�б�־���ĺ󼴷٣�
        case 3: break; //3=�У����ĺ󼴷�
        default: //����
            initflag = true;
            break;
    }

    if(initflag)//�ϵ縴λ��ʼ��
    {
        Iboot_App_Info.PreviouReset = 0;//��λǰ����ģʽ
        Iboot_App_Info.runflag.heard_set_run_iboot   = 0;//Ӳ����������iboot
        Iboot_App_Info.runflag.restart_run_iboot     = 0;//ָʾ����������Iboot
        Iboot_App_Info.runflag.restart_run_app       = 0;//ָʾ����������APP
        Iboot_App_Info.runflag.runmode_iboot         = 0;//��ǰ����ģʽ��iboot
        Iboot_App_Info.runflag.runmode_app           = 0;//��ǰ����ģʽΪapp
        Iboot_App_Info.runflag.Before_run_iboot      = 0;//֮ǰ����ģʽΪiboot
        Iboot_App_Info.runflag.Before_run_app        = 0;//֮ǰ����ģʽΪapp
        #if (CFG_APP_RUNMODE == EN_FORM_FILE)
                Iboot_App_Info.runflag.run_app_form_file     = 1;//���ļ��м���app
        #elif (CFG_APP_RUNMODE == EN_DIRECT_RUN)
                Iboot_App_Info.runflag.run_app_form_file     = 0;//���ļ��м���app
        #endif
        Iboot_App_Info.runflag.run_iboot_update_app  = 0;//������Iboot�����Զ�����APP
        Iboot_App_Info.runflag.run_iboot_update_iboot= 0;//��iboot������iboot����
        Iboot_App_Info.runflag.update_from           = 0;//�����ļ���Դ0�ļ� 1����3������
        Iboot_App_Info.runflag.update_runmode        = 0;//������ɺ�����0.iboot --  1.app
        Iboot_App_Info.runflag.error_app_check       = 0;//У�����
        Iboot_App_Info.runflag.error_app_no_file     = 0;//û������ļ����ļ���ʽ����
        Iboot_App_Info.runflag.error_app_size        = 0;//app�ļ���С����
        Iboot_App_Info.runflag.power_on_flag         = headflag;//�ϵ縴λӲ����־0=�޴�Ӳ����1=�д�Ӳ�������ޱ�־��2=�б�־���ĺ󼴷٣�3=�У����ĺ󼴷٣�
        Iboot_App_Info.runflag.head_wdt_reset        = 0;//���Ź���λ��־
        Iboot_App_Info.runflag.soft_reset_flag       = 0;//����������ڲ���λ
        Iboot_App_Info.runflag.reboot_flag           = 0;//reboot ��־
        Iboot_App_Info.runflag.head_reset_flag       = 0;//�ⲿӲ����λ��־
        Iboot_App_Info.runflag.instructions_reset    = 0;//ָ������ĸ�λ
        Iboot_App_Info.runflag.low_power_wakeup      = 0;//�͹�����������жϻ��ѱ�־
        Iboot_App_Info.runflag.low_power_wakeup      = 0;//�͹�����������жϻ��ѱ�־
        Iboot_App_Info.runflag.call_fun_resent       = 0;//1=�ڲ���λ/����������������غ��������ģ�0=�쳣����
        Iboot_App_Info.runflag.power_on_resent_flag  = 1;//�ϵ縴λ��־�����b18~19�Լ����ϵ��־�����ж�

        Iboot_App_Info.reserved = 0;//����

        Get_Bulid_Time(&Iboot_App_Info.buildyear,&Iboot_App_Info.buildmon,&Iboot_App_Info.buildmday,\
        &Iboot_App_Info.buildhour,&Iboot_App_Info.buildmin,&Iboot_App_Info.buildsec);

        Iboot_App_Info.ibootVer = IBOOT_APP_INFO_VER;         //iboot �汾
        Iboot_App_Info.ibootstartaddr = Init_Cpu;   //iboot������ַ
        Iboot_App_Info.ibootisdebug   = BUILD_IS_DEBUG;   //iboot������ַ

        Fill_boardname(DJY_BOARD,Iboot_App_Info.boardname,sizeof(Iboot_App_Info.boardname));
    }
    else//���ϵ縴λ
    {
        Iboot_App_Info.runflag.power_on_flag = headflag;
        Iboot_App_Info.runflag.power_on_resent_flag  = 0;
        if(Get_Headflag(HEAD_RESET_FLAG))
            Iboot_App_Info.runflag.head_reset_flag = 1;
        else
            Iboot_App_Info.runflag.head_reset_flag = 0;
        if(Get_Headflag(HEAD_WDT_RESET))
            Iboot_App_Info.runflag.head_wdt_reset = 1;
        else
            Iboot_App_Info.runflag.head_wdt_reset = 0;
        if(Get_Headflag(LOW_POWER_WAKEUP))
            Iboot_App_Info.runflag.low_power_wakeup = 1;
        else
            Iboot_App_Info.runflag.low_power_wakeup = 0;

        if(Iboot_App_Info.PreviouReset == PREVIOURESET_APP)
        {
            Iboot_App_Info.runflag.Before_run_app   = 1;
            Iboot_App_Info.runflag.Before_run_iboot= 0;
        }
        else if(Iboot_App_Info.PreviouReset == PREVIOURESET_IBOOT)
        {
            Iboot_App_Info.runflag.Before_run_app = 0;
            Iboot_App_Info.runflag.Before_run_iboot= 1;
        }
        else
        {
            return false;
        }
    }
    return true;
}

//=============================================================================
//���ܣ���������ģʽ�����Ϣ
//������
//���أ�
//=============================================================================
bool_t Run_Iboot(enum runibootmode mode)
{

    Iboot_App_Info.PreviouReset = PREVIOURESET_IBOOT;
    Iboot_App_Info.runflag.heard_set_run_iboot  = 0;
    Iboot_App_Info.runflag.restart_run_iboot    = 0;
    Iboot_App_Info.runflag.reboot_flag    = 0;
    Iboot_App_Info.runflag.runmode_iboot =1;
    Iboot_App_Info.runflag.runmode_app   =0;
    switch(mode)
    {
        case HEARD_SET_RUN_IBOOT :
            Iboot_App_Info.runflag.heard_set_run_iboot  = 1;
            break;
        case RAM_SET_RUN_IBOOT   :
            Iboot_App_Info.runflag.reboot_flag    = 1;
            break;
        case CHACK_ERROR :
            break;
        case UPDATE_APP_RUN_IBOOT:

            break;
        default : break;
    }
    Load_Preload();
    return false;
}

//=============================================================================
//���ܣ��ļ����������������־ִ����Ӧ��iboot��APP
//������
//���أ�
//=============================================================================
bool_t Update_ToRun()
{

    if(Iboot_App_Info.runflag.update_runmode ==0)//iboot
    {

        Iboot_App_Info.runflag.run_iboot_update_iboot = 0;
        Iboot_App_Info.runflag.restart_run_iboot = 1;
    }
    else
    {
        Iboot_App_Info.runflag.restart_run_app = 1;
    }
    reboot();
    return false;
}

//==============================================================================
//���ܣ�������������µ�һЩ��־��λ��Ϣ��־shell����
//������null
//����ֵ��true
//==============================================================================
bool_t clear_resentflag()
{
    Iboot_App_Info.runflag.soft_reset_flag = 0;
    Iboot_App_Info.runflag.instructions_reset = 0;//ָ������ĸ�λ
    Iboot_App_Info.runflag.call_fun_resent = 0;//ָ������ĸ�λ
    return true;
}

//==============================================================================
//���ܣ���������iboot�ı�ע
//������null
//����ֵ��true
//==============================================================================
bool_t Set_RunIbootFlag()
{
    Iboot_App_Info.runflag.restart_run_iboot =1;
    return true;
}

//==============================================================================
//���ܣ���������APP�ı�ע
//������null
//����ֵ��true
//==============================================================================
bool_t Set_RunAppFlag()
{
    Iboot_App_Info.runflag.restart_run_app =1;
    return true;
}

//==============================================================================
//���ܣ���������iboot������app
//������null
//����ֵ��true
//==============================================================================
bool_t Set_RunIbootUpdateApp()
{
    Iboot_App_Info.runflag.run_iboot_update_app =1;
    return true;
}

//==============================================================================
//���ܣ���������iboot������iboot
//������null
//����ֵ��true
//==============================================================================
bool_t Set_RunIbootUpdateIboot()
{
    Iboot_App_Info.runflag.run_iboot_update_iboot =1;
    return true;
}

//==============================================================================
//���ܣ����ø��º�����ģʽ
//������mode 0 ���º�����iboot 1���º�����APP
//����ֵ��true
//==============================================================================
bool_t Set_UpdateRunModet(u8 mode)
{
    if(mode)
        Iboot_App_Info.runflag.update_runmode =1;
    else
        Iboot_App_Info.runflag.update_runmode =0;

    return true;
}

//========================SHELL================================================
static bool_t ibootinfo( )
{
    printf("iboot version:V%2d \n\r",Iboot_App_Info.ibootVer);
    printf("iboot build time : %4d/%2d/%2d %2d:%2d:%2d \n\r",Iboot_App_Info.buildyear,\
            Iboot_App_Info.buildmon, Iboot_App_Info.buildmday, Iboot_App_Info.buildhour, \
            Iboot_App_Info.buildmin, Iboot_App_Info.buildsec);
    printf("Iboot start addr: 0x%8llx \n\r", Iboot_App_Info.ibootstartaddr);
    printf("board name %s \n\r", Iboot_App_Info.boardname);

    return true;
}


static bool_t appinfo( )
{
    struct AppHead*  p_apphead = gc_pAppOffset;
    if(p_apphead->djyflag[0]!='d' || p_apphead->djyflag[1]!='j' || p_apphead->djyflag[2]!='y' )
    {
        printf("cand find APP \r\n");
        return true;
    }
    printf("app ver :v%d \r\n",p_apphead->AppVer);
    printf("app size: %f k \r\n",(float)p_apphead->filesize/1024);
    if(p_apphead->appname[0] == 0)
        printf("app name: NULL  \r\n");
    else
        printf("app name: %s  \r\n",p_apphead->appname);
    printf("app appheadsize: %d  \r\n",p_apphead->appheadsize);

    return true;
}

static bool_t iapmode( )
{

    if(Iboot_App_Info.runflag.heard_set_run_iboot)
        printf( "Ӳ������������iboot\n\r");
    if(Iboot_App_Info.runflag.restart_run_iboot)
        printf( "ָʾ����������Iboot   \r\n");
    if(Iboot_App_Info.runflag.restart_run_app)
        printf( "ָʾ����������APP    \r\n");
    if(Iboot_App_Info.runflag.runmode_iboot)
        printf( "��ǰ����ģʽ��iboot  \r\n");
    if(Iboot_App_Info.runflag.runmode_app  )
        printf( "��ǰ����ģʽΪapp  \r\n");
    if(Iboot_App_Info.runflag.Before_run_iboot)
        printf( "֮ǰ����ģʽΪiboot \r\n");
    if(Iboot_App_Info.runflag.Before_run_app  )
        printf( "֮ǰ����ģʽΪapp  \r\n");
    if(Iboot_App_Info.runflag.run_app_form_file)
        printf( "����ģʽ�����ļ��м���app  \r\n");
    if(Iboot_App_Info.runflag.run_iboot_update_app)
        printf( "������Iboot�����Զ�����APP    \r\n");
    if(Iboot_App_Info.runflag.run_iboot_update_iboot)
        printf( "��iboot������iboot����          \r\n");
    if(Iboot_App_Info.runflag.update_from)
        printf( "�����ļ���Դ0�ļ� 1����3������  \r\n");
    if(Iboot_App_Info.runflag.update_runmode        )
        printf( "������ɺ�����0.iboot --  1.app \r\n");
    if(Iboot_App_Info.runflag.error_app_check       )
        printf( "У�����     \r\n");
    if(Iboot_App_Info.runflag.error_app_no_file     )
        printf( "û������ļ����ļ���ʽ����      \r\n");
    if(Iboot_App_Info.runflag.error_app_size  )
        printf( "App�ļ���С����   \r\n");
     //�ϵ縴λӲ����־0=�޴�Ӳ����1=�д�Ӳ�������ޱ�־��2=�б�־���ĺ󼴷٣�3=�У����ĺ󼴷٣�
    if(Iboot_App_Info.runflag.head_wdt_reset)
    //���Ź���λ��־
    if(Iboot_App_Info.runflag.soft_reset_flag)
        printf( "����������ڲ���λ \r\n");
    if(Iboot_App_Info.runflag.reboot_flag)
        printf( "reboot ��־ \r\n");
    if(Iboot_App_Info.runflag.head_reset_flag)
        printf( "�ⲿӲ����λ��־ \r\n");
    if(Iboot_App_Info.runflag.instructions_reset)
        printf( "ָ������ĸ�λ \r\n");
    if(Iboot_App_Info.runflag.low_power_wakeup)
        printf( "�͹�����������жϻ��ѱ�־ \r\n");
    if(Iboot_App_Info.runflag.call_fun_resent)
        printf( "1=�ڲ���λ/����������������غ��������ģ�0=�쳣���� \r\n");
    if(Iboot_App_Info.runflag.power_on_resent_flag )
        printf( "�ϵ縴λ��־ \r\n");

    return true;
}

ADD_TO_ROUTINE_SHELL(ibootinfo,ibootinfo,NULL);
ADD_TO_ROUTINE_SHELL(appinfo,appinfo,NULL);
ADD_TO_ROUTINE_SHELL(iapmode,iapmode,NULL);

