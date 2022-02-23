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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
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
#include <Iboot_info.h>
#include "stdint.h"
#include "stddef.h"
#include "board-config.h"
#include "component_config_iboot.h"
//-----------------------------------------------------------------------------
//所属模块: si iboot模块
//作者:  czz
//版本：V1.0.0
//文件描述:实现APP对应的bin文件格式
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "shell.h"
#include "string.h"
#include "stdlib.h"
#include <dbug.h>
#include <xip.h>

//extern void AppStart(void);
//extern void Init_Cpu(void);
//extern void CPU_Reboot();
extern void Iboot_LoadPreload(void);
extern void * gc_pAppOffset;
extern void __asm_bl_fun(void * fun_addr);
//版本号
#define APP_HEAD_VERSION        2


#if (CFG_RUNMODE_BAREAPP == 0)
const struct AppHead Djy_App_Head __attribute__ ((section(".DjyAppHead"))) =
{
        .djy_flag[0]    = 'd',
        .djy_flag[1]    = 'j',
        .djy_flag[2]    = 'y',
        .app_ver        = APP_HEAD_VERSION,
        .file_size      = 0xffffffff,
        .verification  = CFG_APP_VERIFICATION,
        .app_bin_size    = 0xffffffff,
#if(CN_PTR_BITS < 64)
        .virt_addr      = (u32)(&Djy_App_Head),
        .reserved32    = 0xffffffff,
#else
        .virt_addr      = (u64)(&Djy_App_Head),
#endif
        .app_head_size   = sizeof(struct AppHead),
        .start_app_is_verify = CFG_START_APP_IS_VERIFICATION,
        .reserved      = {0xff,0xff,0xff,0xff,0xff},
        .app_name       = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
                          0xff,0xff,0xff,0xff,0xff,0xff},

        .verif_buf      = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,\
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
                          0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
};

#endif      //for (CFG_RUNMODE_BAREAPP == 0)
struct IbootAppInfo Iboot_App_Info __attribute__ ((section(".IbootAppInfo"))) ;

//-----------------------------------------------------------------------------
//功能：获取本次启动的复位方式
//参数：ret,上电复位标志寄存器的值。
//返回：上电复位标志
//-----------------------------------------------------------------------------
__attribute__((weak))  enum power_on_reset_flag  Get_PowerOnMode(s8 *ret)
{
    u8 res;
    res = RESET_FLAG_NO_HARD_FLAG;       //实际返回什么根据具体硬件来写
   *ret = 0;                            //本标志应该来自具体硬件
//  有此硬件但读时不自动清零的话，就要在这里把硬件寄存器清零
    return res;
}

//==============================================================================
//功能：获取硬件上的标志，需要用户根据不同的环境自己编写，写在initcpuc.c文件当中
//参数：HEAD_RESET_FLAG： 获取硬件复位标志，没有/不支持返回0
//     HEAD_WDT_RESET： 获取硬件看门狗复位标志，没有/不支持返回0
//     LOW_POWER_WAKEUP： 低功耗唤醒，没有/不支持返回0
//     POWER_ON_RESET：通过硬件获取是否上电复位
//返回：ret：1：是对应flag产生的复位，0：不是对应flag产生的复位，存实际的标志值，
//      0表示上电复位，其他值表示非上电复位。若CPU有此硬件标志才调用）
//==============================================================================
__attribute__((weak))  u8  Get_Hard_flag(enum hardflag flag)
{
    u8 ret;
    //如果是有标志阅后即焚的话，退出该函数时，需要保证该标志真的焚了
    switch (flag)
    {
       case HARD_RESET_FLAG:  ret = 0; break;
       case HARD_WDT_RESET :  ret = 0; break;
       case LOW_POWER_WAKEUP: ret = 0; break;
       case POWER_ON_RESET:
           ret = 0;        //要根据实际情况来
           break;
       default:               break;
    }
    return ret;
}


//==============================================================================
//检查APP的版本是debug
//
//==============================================================================
//bool_t Chack_AppTypeIsDebug(void * apphead)
//{
//    struct AppHead*  p_apphead = apphead;
//
//    if(p_apphead->verification&1)
//        return true;
//    return false;
//}
//=============================================================================
//=============================================================================
//#if (CFG_APP_VERIFICATION == VERIFICATION_CRC)
// CRC32查询表
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
#if (CFG_RUNMODE_BAREAPP == 0)
// ============================================================================
// 功能：CRC32分步计算初始化
// 参数：crc -- CRC值
// 返回：0 -- 失败；1 -- 成功；
// 备注：
// ============================================================================
static s32 iboot_Crc32init(u32 *crc)
{
    if(!crc)
        return (0);
    *crc = -1;
    return (1);
}

// ============================================================================
// 功能：CRC32分步计算
// 参数：crc -- CRC值；buf -- 数据；len -- 数据长度；
// 返回：0 -- 失败；1 -- 成功；
// 备注：
// ============================================================================
static s32 iboot_Crc32run(u32 *crc, u8 *buf, u32 len)
{
    u32 i;
    u32 tmp;

    if((!crc) || (!buf))
        return (0);

    tmp = *crc;
    for(i = 0; i < len;i++)
        tmp = crc32_tab[(tmp ^ buf[i]) & 0xFF] ^ (tmp >> 8);

    *crc = tmp;

    return (1);
}

// ============================================================================
// 功能：CRC32分步计算退出
// 参数：crc -- CRC值；
// 返回：0 -- 失败；1 -- 成功；
// 备注：
// ============================================================================
static s32 iboot_Crc32exit(u32 *crc)
{
    u32 tmp;

    if(!crc)
        return (0);

    tmp = *crc;
    tmp = ~tmp;
    *crc = tmp;

    return (1);
}
#endif
//#elif( CFG_APP_VERIFICATION  == VERIFICATION_MD5 )
//define for the data types
typedef unsigned int u32_t;

/* Data structure for MD5 (Message-Digest) computation */
typedef struct {
  u32_t i[2];               /* number of _bits_ handled mod 2^64 */
  u32_t buf[4];             /* scratch buffer */
  unsigned char in[64];     /* input buffer */
  unsigned char digest[16]; /* actual digest after MD5_Final call */
} MD5_CTX;

/*
 ***********************************************************************
 **  Message-digest routines:                                         **
 **  To form the message digest for a message M                       **
 **    (1) Initialize a context buffer mdContext using MD5_Init        **
 **    (2) Call MD5_Update on mdContext and M                          **
 **    (3) Call MD5_Final on mdContext                                 **
 **  The message digest is now in mdContext->digest[0...15]           **
 ***********************************************************************
 */

/* forward declaration */
static void MD5_Transform (u32_t *buf, u32_t *in);

static unsigned char PADDING[64];
static const unsigned char PADDINGbak[64] = {
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

/* The routine MD5_Init initializes the message-digest context
   mdContext. All fields are set to zero.
 */
static void MD5_Init (MD5_CTX *mdContext)
{
    for(int i=0;i<(int)(sizeof(PADDINGbak)/sizeof(unsigned char));i++ )
        PADDING[i] = PADDINGbak[i];
  mdContext->i[0] = mdContext->i[1] = (u32_t)0;

  /* Load magic initialization constants. */
  mdContext->buf[0] = (u32_t)0x67452301UL;
  mdContext->buf[1] = (u32_t)0xefcdab89UL;
  mdContext->buf[2] = (u32_t)0x98badcfeUL;
  mdContext->buf[3] = (u32_t)0x10325476UL;
}

/* The routine MD5_Update updates the message-digest context to
   account for the presence of each of the characters inBuf[0..inLen-1]
   in the message whose digest is being computed.
 */
static void MD5_Update(MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen)
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
      MD5_Transform (mdContext->buf, in);
      mdi = 0;
    }
  }
}

/* The routine MD5_Final terminates the message-digest computation and
   ends with the desired message digest in mdContext->digest[0...15].
 */
static void MD5_Final (unsigned char hash[], MD5_CTX *mdContext)
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
  MD5_Update (mdContext, PADDING, padLen);

  /* append length in bits and transform */
  for (i = 0, ii = 0; i < 14; i++, ii += 4) {
    in[i] = (((u32_t)mdContext->in[ii+3]) << 24) |
            (((u32_t)mdContext->in[ii+2]) << 16) |
            (((u32_t)mdContext->in[ii+1]) << 8)  |
            ((u32_t)mdContext->in[ii]);
  }
  MD5_Transform (mdContext->buf, in);

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
static void MD5_Transform (u32_t *buf, u32_t *in)
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

//#elif( CFG_APP_VERIFICATION  == VERIFICATION_SSL )
//
//#elif (CFG_APP_VERIFICATION  == VERIFICATION_NULL)

//#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#if (CFG_RUNMODE_BAREAPP == 0)

//-----------------------------------------------------------------------------
//功能：APP文件分步校验之：完成校验码计算，参见 Iboot_CheckAppHead 函数的注释
//参数：apphead：djyapp.bin的文件信息头
//返回：无
//-----------------------------------------------------------------------------
//原名：Iboot_VerificationAppExit
void Iboot_CheckAppComplete(void * apphead)
{
    struct AppHead*  p_apphead = apphead;

    if(p_apphead->verification == VERIFICATION_CRC)
    {
        iboot_Crc32exit((u32*)p_apphead->verif_buf);
    }
    else if(p_apphead->verification == VERIFICATION_MD5)
    {
        unsigned char hash[16];
        u32 i;

        MD5_Final(hash, (MD5_CTX*)p_apphead->verif_buf);
        for(i =0;i<sizeof(MD5_CTX);i++)
            p_apphead->verif_buf[i]=0xff;
        for(i =0;i<16;i++)
            p_apphead->verif_buf[i]=hash[i];
    }
    else if(p_apphead->verification == VERIFICATION_SSL)
    {

    }
}

//-----------------------------------------------------------------------------
//功能：APP文件分步校验之：分批校验文件体，参见 Iboot_CheckAppHead 函数的注释
//参数：apphead：djyapp.bin的文件信息头
//      buf，本次校验的文件体部分
//      len，本次校验的数据长度
//返回：无
//-----------------------------------------------------------------------------
//原名：Iboot_VerificationAppRun
void Iboot_CheckAppBody(void * apphead, u8 * buf, u32 len)
{
    struct AppHead*  p_apphead = apphead;

    if(p_apphead->verification == VERIFICATION_CRC)
    {
        iboot_Crc32run((u32*)p_apphead->verif_buf,buf,len);
    }
    else if(p_apphead->verification == VERIFICATION_MD5)
    {
        MD5_Update((MD5_CTX*)p_apphead->verif_buf, buf,len);
    }
    else if(p_apphead->verification == VERIFICATION_SSL)
    {

    }
}

#pragma GCC diagnostic pop

//-----------------------------------------------------------------------------
//功能：APP文件分步校验之：校验文件头部分，因嵌入式平台内存有限，许多时候不能一次性
//      把整个文件读进内存中，故分步进行。各步骤的计算中间结果，都会保存在APPhead中。
//      step1：计算文件头，包括APP头和产品信息头，Iboot_CheckAppHead 函数完成
//      step2：分步计算剩下的整个文件，直到文件结束， Iboot_CheckAppBody 函数完成
//      step3：完成校验码计算， Iboot_CheckAppComplete 函数完成
//      step4：比较校验结果， Iboot_CheckAppCompare 函数完成
//参数：apphead：djyapp.bin的文件信息头
//返回：true = 需要校验；false=不需要校验，此时上层调用者只须检查文件长度
//-----------------------------------------------------------------------------
//原名：Iboot_VerificationAppInit
bool_t Iboot_CheckAppHead(void *apphead)
{
    struct AppHead*  p_apphead = apphead;
    struct ProductInfo* p_productinfo = apphead + sizeof(struct AppHead);
    u32 i;

    //DIDE中计算校验码时，生产序号使用的是代码中填充的0x2a（即*号）
    for( i=0;i<sizeof(p_productinfo->ProductionNumber);i++)
        p_productinfo->ProductionNumber[i]=0x2a;

    //DIDE中计算校验码时，生产时间使用的是代码中填充的0x2a（即*号）
    for( i=0;i<sizeof(p_productinfo->ProductionTime);i++)
        p_productinfo->ProductionTime[i]=0x2a;

    //DIDE中计算校验码时，app文件名使用的是代码中填充的0xff
    for( i=0;i<sizeof(p_apphead->app_name);i++)
        p_apphead->app_name[i]=0xff;

    //DIDE中计算校验码时，校验码使用的是代码中填充的0xff
    for(u32 i =0;i<sizeof(p_apphead->verif_buf);i++)
        p_apphead->verif_buf[i]=0xff;

    if(p_apphead->verification == VERIFICATION_NULL)
        return false;
    else if(p_apphead->verification == VERIFICATION_CRC)
    {
        iboot_Crc32init((u32*)p_apphead->verif_buf);
        iboot_Crc32run((u32*)p_apphead->verif_buf,apphead,sizeof(struct AppHead) + sizeof(struct ProductInfo));
    }
    else if(p_apphead->verification == VERIFICATION_MD5)
    {
        MD5_CTX md5tmp;
        char *buf = (char *)&md5tmp;
        memset(&md5tmp, 0xff, sizeof(MD5_CTX));
        MD5_Init(&md5tmp);
        MD5_Update(&md5tmp, apphead, sizeof(struct AppHead) + sizeof(struct ProductInfo));
        for(u32 i =0;i< sizeof(MD5_CTX);i++)
            p_apphead->verif_buf[i]=buf[i];
    }
    else if(p_apphead->verification == VERIFICATION_SSL)
    {
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
//功能：APP文件分步校验之：校验文件头部分，参见 Iboot_CheckAppHead 函数的注释
//参数：apphead：djyapp.bin的文件信息头
//      apphead_back，备份的文件信息头
//返回：true = 校验通过；false=校验失败
//-----------------------------------------------------------------------------
//原名：Iboot_VerificationCompare
bool_t  Iboot_CheckAppCompare(void *apphead,void *apphead_back)
{
    struct AppHead*  p_apphead = apphead;
    struct AppHead*  p_appheadcmp = apphead_back;
    u32 i;
    for(i=0;i<sizeof(p_apphead->verif_buf);i++)
    {
        if(p_apphead->verif_buf[i]!=p_appheadcmp->verif_buf[i]
                    &&p_appheadcmp->verif_buf[i]!=(char)0xFF)
            return false;
    }
    return true;
}
//==============================================================================
//功能：重写APP文件信息块的文件信息。
//参数：apphead：App信息块地址；
//      name：新的名字；
//      filesize：新的文件大小
//返回：true：成功；false：失败。
//==============================================================================
bool_t Iboot_RewriteAppHeadFileInfo(void * apphead,const char*name,u32 filesize)
{
    struct AppHead*  p_apphead = apphead;
    u8 flag = 1;
    for(u8 i=0;i<sizeof(p_apphead->app_name);i++)
    {
        if(flag)
            p_apphead->app_name[i] = name[i];
        else
            p_apphead->app_name[i] = 0xff;
        if(name[i]==0)
            flag=0;
    }
    p_apphead->file_size = filesize;
    if(flag)
        return false;
    return true;
}

//==============================================================================
//功能：重写APP产品信息中的产品序号和生产时间。
//参数：productinfo：APP产品信息地址；num：产品序号；time：生产时间
//返回：true：成功；false：失败。
//==============================================================================
//bool_t Iboot_RewriteProductInfoNumTime(void * productinfo,const char* time,const char *num)
//{
//    struct ProductInfo* p_productinfo = productinfo;
//
//    memcpy(p_productinfo->ProductionNumber, num, sizeof(p_productinfo->ProductionNumber));
//    memcpy(p_productinfo->ProductionTime, time, sizeof(p_productinfo->ProductionTime));
//
//    return true;
//}


//==============================================================================
//功能：获取APP文件信息块。
//参数：apphead：存App信息块的地址；
//返回：true：成功；false：失败。
//==============================================================================
bool_t Iboot_GetAppHead(void * apphead)
{
    struct AppHead*  p_apphead = apphead;

    memcpy(p_apphead, gc_pAppOffset,  sizeof(struct AppHead));

    return true;
}

//==============================================================================
//功能：获取APP文件里的产品信息。
//参数：type：app文件头中信息的类型,
//      date_buf：存获取到数据的缓存,
//      buf_len：date_buf的长度
//返回：true：成功；false：失败。
//==============================================================================
bool_t Iboot_GetProductInfo(enum productinfo type, char *date_buf, u32 buf_len)
{
    u32 len;
    struct ProductInfo *p_productinfo;
    p_productinfo = (struct ProductInfo *)(gc_pAppOffset + sizeof(struct AppHead));

    switch(type)
    {
        case APP_HEAD_VERSION_NUM:
        {
            len = sizeof(p_productinfo->VersionNumber) * 3;
            if((p_productinfo->VersionNumber[0] <= 99) ||
                    (p_productinfo->VersionNumber[1] <= 99) || (p_productinfo->VersionNumber[2] <= 99))
            {
                if(buf_len >= len)
                    sprintf(date_buf, "%d.%d.%d", ((char *)p_productinfo->VersionNumber)[0],
                            ((char *)p_productinfo->VersionNumber)[1] ,((char *)p_productinfo->VersionNumber)[2]);
                else
                    goto len_error;
            }
            else
            {
                error_printf("PrInfo","Version number error. version = %d.%d.%d, Version number <= 99",p_productinfo->VersionNumber[0]
                                             ,p_productinfo->VersionNumber[1],p_productinfo->VersionNumber[2]);
                return false;
            }
            break;
        }
        case APP_HEAD_RAW_VERSION_NUM:
        {
            len = sizeof(p_productinfo->VersionNumber);
            if(buf_len >= len)
            {
                memcpy(date_buf, p_productinfo->VersionNumber, len);
            }
            else
                goto len_error;
            break;
        }
        case APP_HEAD_MANUFACTURER:
        {
            len = strlen((char *)p_productinfo->ManufacturerNameAddr);
            if(buf_len >= len)
                memcpy(date_buf, (void *)p_productinfo->ManufacturerNameAddr, len);
            else
                goto len_error;
            break;
        }

        case APP_HEAD_CLASSIFY:
        {
            len = sizeof(p_productinfo->ProductClassify);
            if(buf_len >= len)
                memcpy(date_buf, p_productinfo->ProductClassify, len);
            else
                goto len_error;
            break;
        }

        case APP_HEAD_TYPE:
        {
            len = sizeof(p_productinfo->ProductType);
            if(buf_len >= len)
                memcpy(date_buf, p_productinfo->ProductType, len);
            else
                goto len_error;

            break;
        }

        case APP_HEAD_TYPE_CODE:
        {
            len = sizeof(p_productinfo->TypeCode);
            if(buf_len >= len)
                memcpy(date_buf, p_productinfo->TypeCode, len);
            else
                goto len_error;
            break;
        }

        case APP_HEAD_PRODUCTION_WEEK:
        {
            len = sizeof(p_productinfo->ProductionTime);
            if(buf_len >= len)
                memcpy(date_buf, p_productinfo->ProductionTime, len);
            else
                goto len_error;

            break;
        }

        case APP_HEAD_PRODUCTION_NUM:
        {
            len = sizeof(p_productinfo->ProductionNumber);
            if(buf_len >= len)
                memcpy(date_buf, p_productinfo->ProductionNumber, len);
            else
                goto len_error;

            break;
        }

        case APP_HEAD_BOARD_TYPE:
        {
            len = sizeof(p_productinfo->BoardType);
            if(buf_len >= len)
                memcpy(date_buf, p_productinfo->BoardType, len);
            else
                goto len_error;

            break;
        }

        case APP_HEAD_CPU_TYPE:
        {
            len = sizeof(p_productinfo->CPU_Type);
            if(buf_len >= len)
                memcpy(date_buf, p_productinfo->CPU_Type, len);
            else
                goto len_error;

            break;
        }

        case APP_HEAD_FINGER:
        {
            int type_code_len, time_len, number_len;

            type_code_len = sizeof(p_productinfo->TypeCode);
            time_len = sizeof(p_productinfo->ProductionTime);
            number_len = sizeof(p_productinfo->ProductionNumber);

            len = type_code_len + time_len + number_len + 1;
            if(buf_len >= len)
            {
                memcpy(date_buf, p_productinfo->TypeCode, type_code_len);

                memcpy(date_buf + type_code_len, p_productinfo->ProductionTime, time_len);
                memcpy(date_buf + type_code_len + time_len, p_productinfo->ProductionNumber, number_len);
                date_buf[len-1] = '\0';
            }
            else
                goto len_error;

            break;
        }
    }

    return true;

len_error:
    error_printf("PrInfo","date_buf : len = %d; data length to be get = %d ",buf_len,len);
    return false;
}

extern u32 gc_ProductSn;

//----------------------------------------------------------------------------
//功能: 写指纹到iboot中，写入前先检查指纹区是否空
//参数: time -- 生产时间缓冲区，
//      num -- 生产序号缓冲区，
//返回: true: 成功； false ： 失败.
//-----------------------------------------------------------------------------
bool_t write_finger_to_iboot(s8 *time, s8 *num)
{
    u8 iboot_sn_buf[16] = {0};
    ptu32_t iboot_sn_addr = 0;
    struct ProductInfo *info;

//  iboot_sn_addr = (u32)(&gc_ProductSn) / 32 * 34;
    iboot_sn_addr = (u32)(&gc_ProductSn);
    if(iboot_sn_addr)
    {
//      djy_flash_read(iboot_sn_addr, iboot_sn_buf, 16);
        memcpy(iboot_sn_buf, iboot_sn_addr, 16);
        if((iboot_sn_buf[0] == 0xff) && ((u8)time[0] != 0xff))
        {   //iboot里没SN，程序里给出SN号，现在写SN
            printf("write SN in iboot.\r\n");
            memcpy(iboot_sn_buf, PRODUCT_PRODUCT_MODEL_CODE, sizeof(info->TypeCode));
            memcpy(iboot_sn_buf + sizeof(info->TypeCode), time, 4);
            memcpy(iboot_sn_buf + sizeof(info->TypeCode) + 4, num, 5);

//            flash_protection_op(0,FLASH_PROTECT_NONE);
            djy_flash_write_ori(iboot_sn_addr, iboot_sn_buf, 16);
//            flash_protection_op(0,FLASH_PROTECT_ALL);
        }
    }

    return true;
}

//----------------------------------------------------------------------------
//功能: 读iboot中的指纹
//参数: finger -- 存指纹缓冲区，len -- 缓冲区长度
//返回: true: 成功； false ： 失败.
//-----------------------------------------------------------------------------
bool_t read_finger_from_iboot(s8 *finger, u32 buf_len)
{
    u32 iboot_sn_addr = 0, len;
    struct ProductInfo *info;

    len = sizeof(info->ProductionTime) + sizeof(info->ProductionNumber) + sizeof(info->TypeCode);

     if(buf_len < len)
     {
         printk("finger buf len is error .\r\n");

         return false;
     }

    iboot_sn_addr = (u32)(&gc_ProductSn) ;
    memcpy(finger, iboot_sn_addr, 16);
//  if(iboot_sn_addr)
//  {
//      djy_flash_read(iboot_sn_addr, finger, len);
//  }
//  else
//      return false;

    return true;
}


//char ProductSN[DjyAppHead_SN_Len];
//==============================================================================
//功能：获取产品SN号。
//参数：
//返回：ProductSN -- SN号数组指针
//==============================================================================
//char * Get_ProductSN(void)
//{
//    struct ProductInfo p_productinfo;
//
//    int type_code_len, time_len, number_len;
//
//    type_code_len = sizeof(p_productinfo->TypeCode);
//    time_len = sizeof(p_productinfo->ProductionTime);
//    number_len = sizeof(p_productinfo->ProductionNumber);
//
//    Iboot_GetProductInfo(&p_productinfo);
//    memset(ProductSN, 0, DjyAppHead_SN_Len);
//
//    memcpy(ProductSN, p_productinfo->TypeCode, type_code_len);
//
//    memcpy(ProductSN + type_code_len, p_productinfo->ProductionTime, time_len);
//
//    memcpy(ProductSN + type_code_len + time_len, p_productinfo->ProductionNumber, number_len);
//
//    return (char *)ProductSN;
//}

//char ProductVersion[6];
//==============================================================================
//功能：获取产品版本号。
//参数：
//返回：ProductVersion -- Version号数组指针
//==============================================================================
//char * Get_ProductVersion(void * productinfo)
//{
//    struct ProductInfo p_productinfo;
//    if(productinfo == NULL)
//        Iboot_GetProductInfo(&p_productinfo);
//    else
//        memcpy(&p_productinfo, productinfo,  sizeof(struct ProductInfo));
//
//    memset(ProductVersion, 0, 6);
//
//    sprintf(ProductVersion, "%d.%d.%d", (int)p_productinfo.VersionNumber[0],
//                (int)p_productinfo.VersionNumber[1] ,(int)p_productinfo.VersionNumber[2]);
//
//    return (char *)ProductVersion;
//}

//==============================================================================
//功能：获取厂商名。
//参数：无
//返回：true：成功；false：失败。
//==============================================================================
//const char * Get_ManufacturerName(void)
//{
//    struct ProductInfo p_productinfo;
//    Iboot_GetProductInfo(&p_productinfo);
//
//    return (const char *)p_productinfo.ManufacturerNameAddr;
//}

//==============================================================================
//功能：获取APP文件信息块的大小
//参数：无
//返回：APP信息头的大小。
//==============================================================================
u32 Iboot_GetAppHeadSize(void)
{
   return (u32)sizeof(struct AppHead) + sizeof(struct ProductInfo);
}


//==============================================================================
//功能：App 文件信息块初步检查
//参数：apphead：App信息块地址
//返回：true：成功；false：失败。
//==============================================================================
bool_t XIP_AppFileCheckEasy(void * apphead)
{
    struct AppHead*  p_apphead = apphead;
    if(p_apphead->djy_flag[0]!='d' || \
       p_apphead->djy_flag[1]!='j' || \
       p_apphead->djy_flag[2]!='y')
    {
        return false;
    }

    return true;
}
//==============================================================================
//功能：App 文件的校验,文件全在内存里的话用这个。
//参数：apphead：App信息块地址
//返回：true：成功；false：失败。
//==============================================================================
//原名： XIP_AppFileCheck
bool_t XIP_CheckAppInMemory(void * apphead)
{
    struct AppHead*  p_apphead = apphead;
    char apphead_and_productinfo[sizeof(struct AppHead)+sizeof(struct ProductInfo)];
    u32 AppHead_Len, ProductInfo_Len;
    AppHead_Len = sizeof(struct AppHead);
    ProductInfo_Len = sizeof(struct ProductInfo);
    if(p_apphead->verification != VERIFICATION_NULL)
    {
        memcpy(apphead_and_productinfo, apphead, sizeof(apphead_and_productinfo));
        Iboot_CheckAppHead(apphead_and_productinfo);
        Iboot_CheckAppBody(apphead_and_productinfo, apphead+AppHead_Len+ProductInfo_Len,
                                p_apphead->app_bin_size-AppHead_Len-ProductInfo_Len);
        Iboot_CheckAppComplete(apphead_and_productinfo);
        if(false == Iboot_CheckAppCompare(apphead,apphead_and_productinfo))
        {
            Iboot_App_Info.runflag.error_app_check = 1;
            return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------
//功能: 对文件系统里的文件进行校验
//参数: path：文件路径
//返回: true: 成功；false ： 失败.
//-----------------------------------------------------------------------------
//原名： app_check_from_fs
bool_t XIP_CheckAppInFile(const char *path)
{
    FILE *fp = NULL;
    struct AppHead *apphead = NULL;
    struct AppHead *apphead_back = NULL;
    u8 *buf = NULL;
    bool_t needcheck;
    u32 file_size = 0, readsize, buf_len = 1024,headsize;
    struct stat file_stat;
    bool_t ret = false;

    if(stat(path,&file_stat) == -1)     //看文件是否存在
    {
        printk("update file stat get fail!\r\n");
        return false;
    }

    fp = fopen(path, "r");
    if(fp == NULL)
    {
        printf("update file open fail!\r\n");
        return false;
    }

    headsize = sizeof(struct AppHead)+sizeof(struct ProductInfo);
    apphead = (struct AppHead *)malloc(buf_len + 2 * headsize);     //存文件头信息
    if(apphead)
    {
        apphead_back = (struct AppHead *)((u32)apphead + headsize);      //文件头信息备份
        buf = (u8*)((u32)apphead_back + headsize);          //用于缓冲数据

        readsize = fread(apphead, 1, headsize, fp);     //读文件头
        if(readsize == headsize)
        {
            memcpy(apphead_back, apphead, readsize);    //校验过程头部信息会别修改，故备份
            needcheck = Iboot_CheckAppHead(apphead);
            file_size = readsize;
            while(1)
            {
                readsize = fread(buf, 1, buf_len, fp);
                //不能使用 struct AppHead的app_bin_size成员，有些特殊的CPU例如 beken（博通）
                //会要求烧录的文件进行转换，使文件尺寸与编译出来的尺寸（app_bin_size)不同。
                file_size += readsize;
                if(readsize)
                {
                    if(needcheck == true)
                    {
                        Iboot_CheckAppBody(apphead, buf, readsize);
                    }
                }
                else
                {   //文件全读完了
                    if(file_size != (u32)file_stat.st_size)
                    {
                        printf("file check : file size error\r\n");
                        ret = false;
                    }
                    else if(needcheck == true)
                    {
                        Iboot_CheckAppComplete(apphead);
                         //比较两个文件头里的校验码
                        if(Iboot_CheckAppCompare(apphead,apphead_back) == false)
                        {
                            printf("file check error\r\n");
                        }
                        else
                            ret = true;
                    }
                    else
                        ret = false;
                    break;
                }
            }
        }
        else
            printf("read file %s fail\r\n", path);

        free(apphead);
    }
    fclose(fp);

    return ret;
}

//==============================================================================
//功能：获取APP的运行地址
//参数：apphead：App信息块地址
//返回：APP的运行地址。
//==============================================================================
void * XIP_GetAppStartAddr(void * apphead)
{
    struct AppHead*  p_apphead = apphead;
    return  (void *)p_apphead->virt_addr;
}
//==============================================================================
//功能：获取通过xip文件系统写入的APP文件大小
//参数：apphead：App信息块地址
//返回：APP信息头的大小，该大小是升级时文件系统读到的大小。
//==============================================================================
u32 XIP_GetAppSize(void * apphead)
{
    struct AppHead*  p_apphead = apphead;
    return  p_apphead->file_size;
}

//bool_t XIP_APPIsDebug(void )
//{
//#if defined(DEBUG)
//    return true;
//#else
//    return false;
//#endif
//}
//==============================================================================
//功能：获取编译生成的APP文件大小
//参数：apphead：App信息块地址
//返回：APP信息头的大小，该大小是app.bin文件的大小，由外部工具填充。
//==============================================================================
u32 Iboot_GetAppSize(void * apphead)
{
    struct AppHead*  p_apphead = apphead;
    return  p_apphead->app_bin_size;
}
//==============================================================================
//功能：获取APP文件文件名
//参数：apphead：App信息块地址
//返回：APP的文件名。
//==============================================================================
char* Iboot_GetAppName(void * apphead)
{
    struct AppHead*  p_apphead = apphead;
    return  p_apphead->app_name;
}



//运行APP
//bool_t File_Run_App(void * apphead)
//{
//    struct AppHead*  p_apphead = apphead;
//    Iboot_App_Info.runflag.runmode_iboot= 0;
//    Iboot_App_Info.runflag.runmode_app  = 1;
//    Iboot_App_Info.runflag.restart_run_app = 0;
//
//    Iboot_App_Info.runflag.error_app_no_file = 0;
//    Iboot_App_Info.runflag.error_app_check = 0;
//    Iboot_App_Info.runflag.error_app_size = 0;
//
//    Iboot_App_Info.runflag.run_iboot_and_update_app = 0;
//
//    __asm_bl_fun((void*)p_apphead+sizeof(struct AppHead));
//    return false;
//}

//==============================================================================
//功能：判断重启后是否强制运行Iboot
//参数：
//返回： true/false
//==============================================================================
bool_t XIP_IsRamIbootFlag()
{
    if(Iboot_App_Info.runflag.restart_run_iboot ==1)
        return true;
    return false;
}

//==============================================================================
//功能：获取iboot编译时间
//参数：年月日时分秒的存放地址
//返回： true/false
//备注：因为要放到启动代码段所以尽量不用外部文件的函数实现
//==============================================================================
static bool_t Iboot_GetIbootBulidTime(u16 *pyear,u8 *pmon,u8 *pday,u8 *phour,u8 *pmin,u8 *psec)
{

    char *time = __TIME__;/*11:35:09*/
    char *date = __DATE__;/*Dec 27 2018*/
    char *p = date;
    *phour = 10*(time[0]-0x30)+(time[1]-0x30);
    *pmin = 10*(time[3]-0x30)+(time[4]-0x30);
    *psec = 10*(time[6]-0x30)+(time[7]-0x30);

    u8 num = 20;
    while(num--)//容错
    {
        if(*p==0)
            return false;
        if(*p == ' ')
            break;
        p++;
    }
    *pday =  10*(p[1]-0x30)+(p[2]-0x30);
    p+=3;
    while(num--)//容错
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
#endif
//==============================================================================
//功能：设置app编译时间
//参数：年月日时分秒
//返回： true/false
//备注：
//==============================================================================
bool_t Iboot_SetAppBulidTime(u16 pyear,u8 pmon,u8 pday,u8 phour,u8 pmin,u8 psec)
{
    if((pyear == 0) || ((pmon <= 0)||(pmon >12)) || ((pday <= 0)||(pday >31))
            || ((phour <= 0)||(phour > 24)) || ((pmin <= 0)||(pmin >60)) || ((psec <= 0)||(psec >60)))
    {
        return false;
    }

    Iboot_App_Info.app_build_year = pyear;
    Iboot_App_Info.app_build_mon = pmon;
    Iboot_App_Info.app_build_day = pday;
    Iboot_App_Info.app_build_hour = phour;
    Iboot_App_Info.app_build_min = pmin;
    Iboot_App_Info.app_build_sec = psec;

    return true;
}
//==============================================================================
//功能：填充板件名到交互信息
//参数：board_name：板件名称；buf：待写入的地址；maxlen：板件名称最大长度。
//返回： true/false
//==============================================================================
static bool_t Iboot_FillBoardName(char* board_name,char* buf,u8 maxlen)
{
    u8 i;
    for(i=0;i<maxlen;i++)
    {
        if(*board_name != 0)
            *buf++ = *board_name++;
        else
        {
            *buf++ = *board_name++;
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
 * 功能    ：si模式初始化iboot与APP共享内存信息
 * 参数    ：NULL
 * 返回值：true/false
==============================================================================*/
bool_t Iboot_SiIbootAppInfoInit()
{
    bool_t PowerUp = false;
    s8 flag = 1;
    enum power_on_reset_flag hardflag = Get_PowerOnMode(&flag);
    switch (hardflag)
    {
        case RESET_FLAG_NO_HARD_FLAG:               //0=无此硬件
            if((Iboot_App_Info.previou_reset != PREVIOURESET_APP) && \
               (Iboot_App_Info.previou_reset != PREVIOURESET_IBOOT))
                PowerUp = true;
            break;
        case RESET_FLAG_NO_FLAG: break;             //1=有此硬件，没有上电复位标志；
        case RESET_FLAG_READ_AND_CLR:               //2=有标志，且读后自动清除
        case RESET_FLAG_READ_NORMAL:                //3=有标志，读后不会自动清除
            if(flag)
                PowerUp = true;
            break;
        default: //错误
            PowerUp = true;
            break;
    }
#if (CFG_RUNMODE_BAREAPP == 0)
    Iboot_GetIbootBulidTime(&Iboot_App_Info.iboot_build_year,&Iboot_App_Info.iboot_build_mon,
            &Iboot_App_Info.iboot_build_day,&Iboot_App_Info.iboot_build_hour,
            &Iboot_App_Info.iboot_build_min,&Iboot_App_Info.iboot_build_sec);
    Iboot_App_Info.iboot_ver_small = CFG_IBOOT_VERSION_SMALL;         //iboot 版本
    Iboot_App_Info.iboot_ver_medium = CFG_IBOOT_VERSION_MEDIUM;         //iboot 版本
    Iboot_App_Info.iboot_ver_large = CFG_IBOOT_VERSION_LARGE;         //iboot 版本
#endif
    Iboot_FillBoardName(PRODUCT_BOARD_TYPE,Iboot_App_Info.board_name,sizeof(Iboot_App_Info.board_name));
    if(PowerUp == true)                        //上电复位初始化
    {
        Iboot_App_Info.previou_reset = 0;//复位前运行模式
        Iboot_App_Info.runflag.hard_set_run_iboot   = 0;//硬件设置运行iboot
        Iboot_App_Info.runflag.restart_run_iboot     = 0;//指示启动后运行Iboot
        Iboot_App_Info.runflag.restart_run_app       = 0;//指示启动后运行APP
        Iboot_App_Info.runflag.runmode_iboot         = 0;//当前运行模式是iboot
        Iboot_App_Info.runflag.runmode_app           = 0;//当前运行模式为app
        Iboot_App_Info.runflag.Before_run_iboot      = 0;//之前运行模式为iboot
        Iboot_App_Info.runflag.Before_run_app        = 0;//之前运行模式为app
        #if (CFG_APP_RUNMODE == CN_RUN_FORM_FILE)
                Iboot_App_Info.runflag.run_app_form_file     = 1;//从文件中加载app
        #elif (CFG_APP_RUNMODE == CN_DIRECT_RUN)
                Iboot_App_Info.runflag.run_app_form_file     = 0;//从文件中加载app
        #endif
        Iboot_App_Info.runflag.run_iboot_and_update_app  = 0;//启动（Iboot）后，自动升级APP
        Iboot_App_Info.runflag.run_app_and_update_iboot  = 0;//启动app后升级iboot自身
        Iboot_App_Info.runflag.update_from           = 0;//升级文件来源0文件 ,1内存,2—3用户自定义
        Iboot_App_Info.runflag.after_update_runmode  = 0;//升级完成后运行0.iboot --  1.app
        Iboot_App_Info.runflag.error_app_check       = 0;//校验出错
        Iboot_App_Info.runflag.error_app_no_file     = 0;//没有这个文件或文件格式错误
        Iboot_App_Info.runflag.error_app_size        = 0;//app文件大小错误
        Iboot_App_Info.runflag.power_on_flag         = hardflag;//上电复位硬件标志0=无此硬件；1=有此硬件，但无标志；2=有标志，阅后即焚；3=有，非阅后即焚；
        Iboot_App_Info.runflag.hard_wdt_reset        = 0;//看门狗复位标志
        Iboot_App_Info.runflag.soft_reset_flag       = 0;//软件引起的内部复位
        Iboot_App_Info.runflag.reboot_flag           = 0;//CPU_Reboot 标志
        Iboot_App_Info.runflag.hard_reset_flag       = 0;//外部硬件复位标志
        Iboot_App_Info.runflag.restart_system_flag      = 0;//restart_system复位标志
        Iboot_App_Info.runflag.low_power_wakeup      = 0;//低功耗深度休眠中断唤醒标志
        Iboot_App_Info.runflag.call_fun_reset       = 0;//1=复位/重启是主动调用相关函数引发的；0=异常重启
        Iboot_App_Info.runflag.power_on_reset_flag  = 1;//上电复位标志，结合b18~19以及“上电标志”字判定

        Iboot_App_Info.UserTag = 0;                     //上电复位，用户标志被清零
        Iboot_App_Info.reserved = 0;//保留
#if (CFG_POWER_ON_RESET_TO_BOOT)
        Iboot_SetRunIbootFlag();
#endif
    }
    else//非上电复位
    {
        Iboot_App_Info.runflag.error_app_no_file     = 0;//没有这个文件或文件格式错误
        Iboot_App_Info.runflag.power_on_flag = hardflag;
        Iboot_App_Info.runflag.power_on_reset_flag  = 0;
        if(Get_Hard_flag(HARD_RESET_FLAG))
            Iboot_App_Info.runflag.hard_reset_flag = 1;
        else
            Iboot_App_Info.runflag.hard_reset_flag = 0;
        if(Get_Hard_flag(HARD_WDT_RESET))
            Iboot_App_Info.runflag.hard_wdt_reset = 1;
        else
            Iboot_App_Info.runflag.hard_wdt_reset = 0;
        if(Get_Hard_flag(LOW_POWER_WAKEUP))
            Iboot_App_Info.runflag.low_power_wakeup = 1;
        else
            Iboot_App_Info.runflag.low_power_wakeup = 0;

        if(Iboot_App_Info.previou_reset == PREVIOURESET_APP)
        {
            Iboot_App_Info.runflag.Before_run_app   = 1;
            Iboot_App_Info.runflag.Before_run_iboot= 0;
        }
        else if(Iboot_App_Info.previou_reset == PREVIOURESET_IBOOT)
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

//==============================================================================
//功能：清除重启不更新的一些标志复位信息标志shell函数
//参数：null
//返回值：true
//==============================================================================
bool_t Iboot_ClearResetFlag()
{
    Iboot_App_Info.runflag.soft_reset_flag = 0;
    Iboot_App_Info.runflag.call_fun_reset = 0;//指令引起的复位
    return true;
}

//==============================================================================
//功能：设置APP版本号
//参数：small -- xx.xx.__; medium -- xx.__.xx; large -- __.xx.xx
//返回值：true
//==============================================================================
bool_t Iboot_SetAppVerFlag(u8 small, u8 medium, u8 large)

{
    Iboot_App_Info.app_ver_small = small;         //app 版本
    Iboot_App_Info.app_ver_medium = medium;       //app 版本
    Iboot_App_Info.app_ver_large = large;         //app 版本

    return true;
}

//-----------------------------------------------------------------------------
//功能：设置用户标签，该标签可用于指示APP在重启后完成一些特定的操作
//参数：UserTag，待设置的标签
//返回：无
//------------------------------------------------------------------------------
void Iboot_SetUserTag(u32 UserTag)
{
    Iboot_App_Info.UserTag = UserTag;
}

//-----------------------------------------------------------------------------
//功能：获取用户标签，该标签可用于指示APP在重启后完成一些特定的操作
//参数：无
//返回：用户标签
//------------------------------------------------------------------------------
u32 Iboot_GetUserTag(void)
{
    return Iboot_App_Info.UserTag;
}

//-----------------------------------------------------------------------------
//功能：设置生产周次，即该产品是哪一年哪一周生产的
//参数：week，指向一个4字节缓冲区
//返回：无
//------------------------------------------------------------------------------
void Iboot_SetWeek(s8 *week)
{
    memcpy(Iboot_App_Info.production_week, week, 4);
}

//-----------------------------------------------------------------------------
//功能：获取生产周次，即该产品是哪一年哪一周生产的
//参数：week，接收结果，指向一个4字节缓冲区
//返回：true = 成功读取week，false=交互信息中week信息空。
//------------------------------------------------------------------------------
bool_t Iboot_GetWeek(s8 *week)
{
    if (Iboot_App_Info.production_week[0] == 0)
        return false;
    else
    {
        memcpy(week, Iboot_App_Info.production_week, 4);
        return true;
    }
}

//-----------------------------------------------------------------------------
//功能：设置生产系列号，即该产品是一周内不重复的生产序号
//参数：serial，指向一个5字节缓冲区
//返回：无
//------------------------------------------------------------------------------
void Iboot_SetSerial(s8 *serial)
{
    memcpy(Iboot_App_Info.production_serial, serial, 5);
}

//-----------------------------------------------------------------------------
//功能：获取生产系列号，即该产品是一周内不重复的生产序号
//参数：serial，接收结果，指向一个5字节缓冲区
//返回：true = 成功读取serial，false=交互信息中serial信息空。
//------------------------------------------------------------------------------
bool_t Iboot_GetSerial(s8 *serial)
{
    if (Iboot_App_Info.production_serial[0] == 0)
        return false;
    else
    {
        memcpy(serial, Iboot_App_Info.production_serial, 5);
        return true;
    }
}

//-----------------------------------------------------------------------------
//功能：设置ota起始地址，ota下载的APP，将保存到这个可寻址的内存中。
//参数：address，保存APP的地址
//      size,app尺寸
//返回：无
//------------------------------------------------------------------------------
void Iboot_SetOtaAddr(s8 *address, ptu32_t size)
{
    Iboot_App_Info.stored.ram.start_add = address;
    Iboot_App_Info.stored.ram.app_size = size;
}

//-----------------------------------------------------------------------------
//功能：获取ota起始地址，ota下载的APP，将保存到这个可寻址的内存中。
//参数：address，接收地址的指针
//      size，接收尺寸的指针
//返回：无
//------------------------------------------------------------------------------
void Iboot_GetOtaAddr(s8 **address, ptu32_t *size)
{
    *address = Iboot_App_Info.stored.ram.start_add;
    *size = Iboot_App_Info.stored.ram.app_size;
}

//-----------------------------------------------------------------------------
//功能：设置ota 的存储参数，共31个字节(CN_APP_STORE_INFO_LIMIT)，存储参数描述ota下载
//      的文件是怎么存储的，如果保存在文件中或可寻址内存中，存储参数的含义由系统解析，
//      用户不用管，其他情况由用户解析，例如，如果APP存储在spi接口的flash中(非文件
//      系统），参数用于约定偏移地址。
//参数：address, 信息地址
//      size, 信息尺寸
//返回：无
//------------------------------------------------------------------------------
void Iboot_SetOtaUserInfo(s8 *address, ptu32_t size)
{
    if (size <= CN_APP_STORE_INFO_LIMIT)
        memcpy(Iboot_App_Info.stored.pads, address,size);
    else
        memcpy(Iboot_App_Info.stored.pads, address,CN_APP_STORE_INFO_LIMIT);
}

//-----------------------------------------------------------------------------
//功能：获取ota 的存储参数，共31个字节(CN_APP_STORE_INFO_LIMIT)，存储参数描述ota下载
//      的文件是怎么存储的，如果保存在文件中或可寻址内存中，存储参数的含义由系统解析，
//      用户不用管，其他情况由用户解析，例如，如果APP存储在spi接口的flash中(非文件
//      系统），参数用于约定偏移地址。
//参数：address, 信息地址
//      size, 信息尺寸
//返回：无
//------------------------------------------------------------------------------
void Iboot_GetOtaUserInfo(s8 *address, ptu32_t size)
{
    if (size <= CN_APP_STORE_INFO_LIMIT)
        memcpy(address, Iboot_App_Info.stored.pads, size);
    else
        memcpy(address, Iboot_App_Info.stored.pads, CN_APP_STORE_INFO_LIMIT);
}

//-----------------------------------------------------------------------------
//功能：设置ota 保存下载文件的文件路径，必须是绝对路径。注意，这个文件名并不是APP编译
//      时的文件名，也不是保存在服务器上的文件名，而是ota把文件下载后，转存到本地的
//      文件名，一般是loader模块配置常量 CFG_APP_UPDATE_NAME。
//参数：filename, 信息地址
//      size, 信息尺寸
//返回：无
//------------------------------------------------------------------------------
void Iboot_SetOtaFilename(s8 *filename, ptu32_t size)
{
    if (size <= CN_APP_STORE_INFO_LIMIT)
        memcpy(Iboot_App_Info.stored.pads, filename,size);
    else
        memcpy(Iboot_App_Info.stored.pads, filename,CN_APP_STORE_INFO_LIMIT);
}

//-----------------------------------------------------------------------------
//功能：获取ota 保存下载文件的文件路径，必须是绝对路径。注意，这个文件名并不是APP编译
//      时的文件名，也不是保存在服务器上的文件名，而是ota把文件下载后，转存到本地的
//      文件名，一般是loader模块配置常量 CFG_APP_UPDATE_NAME。
//参数：filename, 信息地址
//      size, 信息尺寸
//返回：无
//------------------------------------------------------------------------------
void Iboot_GetOtaFilename(s8 *filename, ptu32_t size)
{
    if (size <= CN_APP_STORE_INFO_LIMIT)
        memcpy(filename, Iboot_App_Info.stored.pads, size);
    else
        memcpy(filename, Iboot_App_Info.stored.pads, CN_APP_STORE_INFO_LIMIT);
}

//==============================================================================
//功能：设置软件内部复位标志
//参数：null
//返回值：true
//==============================================================================
bool_t Iboot_SetSoftResetFlag()
{
    Iboot_App_Info.runflag.call_fun_reset = 1;
    Iboot_App_Info.runflag.soft_reset_flag = 1;
    Iboot_App_Info.runflag.reboot_flag = 0;
    Iboot_App_Info.runflag.restart_system_flag = 0;
    return true;
}
//==============================================================================
//功能：设置reboot复位标志
//参数：null
//返回值：true
//==============================================================================
bool_t Iboot_SetRebootFlag()
{
    Iboot_App_Info.runflag.call_fun_reset = 1;
    Iboot_App_Info.runflag.reboot_flag = 1;
    Iboot_App_Info.runflag.restart_system_flag = 0;
    return true;
}
//==============================================================================
//功能：设置restart_system复位标志
//参数：null
//返回值：true
//==============================================================================
bool_t Iboot_SetRestartAppFlag()
{
    Iboot_App_Info.runflag.call_fun_reset = 1;
    Iboot_App_Info.runflag.restart_system_flag = 1;
    return true;
}

//==============================================================================
//功能：获取是否为异常重启
//参数：无
//返回值：true -- 是；false -- 否
//==============================================================================
bool_t Iboot_GetCallFunResent()
{
    if(Iboot_App_Info.runflag.call_fun_reset == 0)
        return true;
    else
        return false;
}

//==============================================================================
//功能：获取是否是软件内部复位标志
//参数：无
//返回值：true -- 是；false -- 否
//==============================================================================
bool_t Iboot_GetSoftResetFlag()
{
    if(Iboot_App_Info.runflag.soft_reset_flag == 1)
        return true;
    else
        return false;
}
//==============================================================================
//功能：获取是否有reboot标志
//参数：无
//返回值：true -- 有；false -- 无
//==============================================================================
bool_t Iboot_GetRebootFlag()
{
    if(Iboot_App_Info.runflag.reboot_flag == 1)
        return true;
    else
        return false;
}
//==============================================================================
//功能：获取是否有restart_system标志
//参数：无
//返回值：true -- 有；false -- 无
//==============================================================================
bool_t Iboot_GetRestartAppFlag()
{
    if(Iboot_App_Info.runflag.restart_system_flag == 1)
        return true;
    else
        return false;
}
//==============================================================================
//功能：获取是否由外部硬件引起的复位
//参数：无
//返回值：true -- 是；false -- 否
//==============================================================================
bool_t Iboot_GetHeadResetFlag()
{
    if(Iboot_App_Info.runflag.hard_reset_flag == 1)
        return true;
    else
        return false;
}
//==============================================================================
//功能：获取低功耗深度休眠是否由中断唤醒
//参数：无
//返回值：true -- 是；false -- 否
//==============================================================================
bool_t Iboot_GetLowPowerWakeup()
{
    if(Iboot_App_Info.runflag.low_power_wakeup == 1)
        return true;
    else
        return false;
}

//==============================================================================
//功能：获取上电复位硬件标志
//参数：无
//返回值：0=无此硬件 1=有此硬件，但无标志；2=有标志，阅后即焚；3=有，非阅后即焚；
//==============================================================================
char Iboot_GetPowerOnFlag(void)
{
    return Iboot_App_Info.runflag.power_on_flag;
}

//==============================================================================
//功能：获取是否由看门狗引起的复位
//参数：无
//返回值：true -- 是；false -- 否
//==============================================================================
bool_t Iboot_GetHeadWdtReset(void)
{
    if(Iboot_App_Info.runflag.hard_wdt_reset == 1)
        return true;
    else
        return false;
}

#if (CFG_RUNMODE_BAREAPP == 0)
//==============================================================================
//功能：填待升级文件路径到交互信息
//参数：AppPath -- 待升级的app路径；
//返回： true/false/
//原名 ： Iboot_FillMutualUpdatePath
//==============================================================================
//bool_t set_upgrade_info(char* info, int len)
//{
////    u8 i;
//
//    if(len > (int)sizeof(Iboot_App_Info.stored))
//    {
//        error_printf("IAP"," len exceed CN_UPDATE_PATH_LIMIT.\r\n");
//        return false;
//    }
//    memset(Iboot_App_Info.stored.info, 0, CN_UPDATE_PATH_LIMIT);
//    memcpy(Iboot_App_Info.stored.info, info, len);
//    if(len == CN_UPDATE_PATH_LIMIT)
//    {
//        Iboot_App_Info.stored.info[CN_UPDATE_PATH_LIMIT-1] = 0;
//        return true;
//    }
//    return true;
//
//
//}
//=============================================================================
//功能：根据启动模式填充信息
//参数：mode：iboot的启动模式
//返回：true/false
//=============================================================================
bool_t Run_Iboot(enum runibootmode mode)
{
    Iboot_App_Info.runflag.hard_set_run_iboot  = 0;
    Iboot_App_Info.runflag.restart_run_iboot    = 0;
//    Iboot_App_Info.runflag.reboot_flag    = 0;
    Iboot_App_Info.runflag.runmode_iboot =1;
    Iboot_App_Info.runflag.runmode_app   =0;
    switch(mode)
    {
        case HARD_SET_RUN_IBOOT :
            Iboot_App_Info.runflag.hard_set_run_iboot  = 1;
            break;
        case RAM_SET_RUN_IBOOT   :
//            Iboot_App_Info.runflag.reboot_flag    = 1;
            break;
        case CHACK_ERROR :
            break;
        case UPDATE_APP_RUN_IBOOT:

            break;
        default : break;
    }
    Iboot_LoadPreload();
    return true;
}

//==============================================================================
//功能：Direct模式运行APP
//参数：apphead：App信息块地址
//返回：true/false
//==============================================================================
static bool_t __RunApp(void * apphead)
{
    struct AppHead*  p_apphead = apphead;
    struct AppHead app_head;

    u8 * buf = (u8*)&app_head;
    u8 *bufapp = apphead;
    u32 i;
    if(p_apphead->djy_flag[0]!='d' ||p_apphead->djy_flag[1]!='j' ||p_apphead->djy_flag[2]!='y' )
    {
        Iboot_App_Info.runflag.error_app_no_file = 1;
        return false;
    }
    for(i=0;i<sizeof(struct AppHead);i++)
        buf[i] = bufapp[i];
    Iboot_App_Info.runflag.restart_run_app = 0;
    Iboot_App_Info.runflag.error_app_check = 0;
    Iboot_App_Info.runflag.error_app_size  = 0;
    if(p_apphead->app_bin_size != p_apphead->file_size)
    {
        Iboot_App_Info.runflag.error_app_size  = 1;
    }
    if((p_apphead->start_app_is_verify) && (p_apphead->verification != VERIFICATION_NULL))
    {
        for(i = 0; i < sizeof(p_apphead->verif_buf); i ++)
        {
            if((u8)p_apphead->verif_buf[i] != 0xff)  //判断检验码缓冲区中是否为全FF，如果为全FF则表示是在debug，不做程序的校验
                break;
        }
        if(i < sizeof(p_apphead->verif_buf))
        {
            if(XIP_CheckAppInMemory(apphead) == false)
                return false;
        }
    }
    Iboot_App_Info.runflag.runmode_iboot        = 0;
    Iboot_App_Info.runflag.runmode_app     = 1;
    __asm_bl_fun((void*)((u32)p_apphead+sizeof(struct AppHead)+sizeof(struct ProductInfo)));
    return true;
}

//==============================================================================
//功能:启动APP
//参数：mode：app的启动模式
//返回：true/false
//==============================================================================
bool_t Run_App(enum runappmode mode)
{
    Iboot_App_Info.runflag.hard_set_run_iboot  = 0;

    switch (mode)
    {
        case RUN_APP_FROM_FILE   :
            Iboot_App_Info.runflag.runmode_iboot        = 0;
            Iboot_App_Info.runflag.runmode_app          = 1;
            Iboot_LoadPreload();
            return true;
        case RUN_APP_FROM_DIRECT :
            return __RunApp((void * )gc_pAppOffset);
        default:          break;
    }
    return false;
}

//=============================================================================
//功能：设置重启后执行iboot或APP
//参数：无
//返回：true/false
//=============================================================================
bool_t Iboot_UpdateToRun()
{

    if(Iboot_App_Info.runflag.after_update_runmode ==0)//iboot
    {
        info_printf("IAP","About to run the iboot.\r\n");
        Iboot_App_Info.runflag.restart_run_iboot = 1;
    }
    else
    {
        info_printf("IAP","About to run the APP.\r\n");
        Iboot_App_Info.runflag.restart_run_app = 1;
    }
    DJY_EventDelay(100*1000);      //延时一下，让升级过程中的信息能打印出来
//  CPU_Reboot();       //类似BK7251的CPU，重启后如果要继续调试，改为reboot
    CPU_Reset();
    return true;
}
//==============================================================================
//功能：设置复位前的运行模式
//参数：null
//返回值：true
//==============================================================================
bool_t Iboot_SetPreviouResetFlag()
{
    if(Iboot_App_Info.runflag.runmode_app == 1)
        Iboot_App_Info.previou_reset = PREVIOURESET_APP;
    if(Iboot_App_Info.runflag.runmode_iboot == 1)
        Iboot_App_Info.previou_reset = PREVIOURESET_IBOOT;

    return true;
}
//==============================================================================
//功能：设置运行iboot的标注
//参数：null
//返回值：true
//==============================================================================
bool_t Iboot_SetRunIbootFlag()
{
    Iboot_App_Info.runflag.restart_run_iboot =1;
    return true;
}

//==============================================================================
//功能：设置运行APP的标注
//参数：null
//返回值：true
//==============================================================================
bool_t Iboot_SetRunAppFlag()
{
    Iboot_App_Info.runflag.restart_run_app =1;
    return true;
}

//==============================================================================
//功能：设置运行iboot并更新app
//参数：null
//返回值：true
//==============================================================================
bool_t Iboot_SetRunIbootAndUpdateApp()
{
    Iboot_App_Info.runflag.run_iboot_and_update_app =1;
    return true;
}

//==============================================================================
//功能：清除运行iboot并更新app标志
//参数：null
//返回值：true
//==============================================================================
bool_t Iboot_ClearRunIbootAndUpdateApp()
{
    Iboot_App_Info.runflag.run_iboot_and_update_app =0;
    return true;
}

//==============================================================================
//功能：设置运行app并更新iboot
//参数：null
//返回值：true
//==============================================================================
bool_t Iboot_SetRunAppAndUpdateIboot()
{
    Iboot_App_Info.runflag.run_app_and_update_iboot =1;
    return true;
}

//==============================================================================
//功能：清除运行app并更新iboot标志
//参数：null
//返回值：true
//==============================================================================
bool_t Iboot_ClearRunAppAndUpdateIboot()
{
    Iboot_App_Info.runflag.run_app_and_update_iboot =0;
    return true;
}

//==============================================================================
//功能：设置升级程序来源
//参数：升级程序来源0文件 1——3待定义
//返回值：true
//==============================================================================
bool_t Iboot_SetUpdateSource(u32 source)
{
    Iboot_App_Info.runflag.update_from = source;
    return true;
}

//==============================================================================
//功能：获取升级程序来源
//参数：无
//返回值：升级程序来源0文件 1——3待定义
//==============================================================================
u32 Iboot_GetUpdateSource(void)
{
     return Iboot_App_Info.runflag.update_from;
}
//==============================================================================
//功能：获取启动后是否运行app
//参数：无
//返回值：true -- 是；false -- 否
//==============================================================================
bool_t Iboot_GetRestartRunApp()
{
    if(Iboot_App_Info.runflag.restart_run_app == 1)
        return true;
    else
        return false;
}

//==============================================================================
//功能：查询APP是否从文件加载执行
//参数：无
//返回值：true = 从文件中加载，false = 非从文件加载（直接运行）
//==============================================================================
bool_t Iboot_IsLoadAppFromFile()
{
    return (Iboot_App_Info.runflag.run_app_form_file == 1);
}

//==============================================================================
//功能：获取当前运行的模式
//参数：无
//返回值：0 -- 运行iboot；1 -- 运行app；-1 -- 错误
//==============================================================================
char Iboot_GetRunMode(void)
{
    if(Iboot_App_Info.runflag.runmode_iboot == 1)
        return 0;
    else
        if(Iboot_App_Info.runflag.runmode_app == 1)
            return 1;

    return -1;
}

//==============================================================================
//功能：获取上一次运行的模式
//参数：无
//返回值：0 -- 运行iboot；1 -- 运行app；-1 -- 错误
//==============================================================================
char Iboot_GetLastRunMode(void)
{
    if(Iboot_App_Info.runflag.Before_run_iboot == 1)
        return 0;
    else
        if(Iboot_App_Info.runflag.Before_run_app == 1)
            return 1;

    return -1;
}
//==============================================================================
//功能：获取是否是由硬件设置运行iboot
//参数：无
//返回值：true -- 不是硬件决定的；false -- 是硬件决定的；
//==============================================================================
bool_t Iboot_GetHeardSetRunIboot(void)
{
    if(Iboot_App_Info.runflag.hard_set_run_iboot == 1)
        return true;
    else
        return false;
}
//==============================================================================
//功能：获取交互信息中的App路径
//参数：buf:存APP路径的缓存，buf_len：缓存长度
//返回值：待升级app路径
//==============================================================================
//被 Iboot_SetOtaFilename 替代
//bool_t Iboot_GetMutualUpdatePath(char *buf, u32 buf_len)
//{
//    u32 len;
//    len = sizeof(Iboot_App_Info.stored);
//    if(buf_len >= len)
//    {
//        memcpy(buf, Iboot_App_Info.stored.info, len);
//        return true;
//    }
//    else
//    {
//        error_printf("Mutual","buf : len = %d; data length to be get = %d ",buf_len,len);
//        return false;
//    }
//}
//==============================================================================
//功能：判断是否需要升级app
//参数：无
//返回值：true -- 需要升级app；false -- 不需要升级app
//==============================================================================
bool_t Iboot_GetUpdateApp(void)
{
    if(Iboot_App_Info.runflag.run_iboot_and_update_app == 1)
        return true;
    else
        return false;
}

//==============================================================================
//功能：判断是否需要升级iboot
//参数：无
//返回值：true -- 需要升级iboot；false -- 不需要升级iboot
//==============================================================================
bool_t Iboot_GetUpdateIboot(void)
{
    if(Iboot_App_Info.runflag.run_app_and_update_iboot == 1)
        return true;
    else
        return false;
}
//==============================================================================
//功能：获取app是否校验出错
//参数：无
//返回值：true -- app校验出错；false -- app检验正确
//==============================================================================
bool_t Iboot_GetErrorAppCheck(void)
{
    if(Iboot_App_Info.runflag.error_app_check == 1)
        return true;
    else
        return false;
}
//==============================================================================
//功能：获取是否存在app文件
//参数：无
//返回值：true -- app不存在或者文件格式错误；false -- app存在且正常
//==============================================================================
bool_t Iboot_GetErrorAppNoFile(void)
{
    if(Iboot_App_Info.runflag.error_app_no_file == 1)
        return true;
    else
        return false;
}
//==============================================================================
//功能：获取是否app文件大小是否正确
//参数：无
//返回值：true -- app文件大小错误；false -- app文件大小正确
//==============================================================================
bool_t Iboot_GetErrorAppSize(void)
{
    if(Iboot_App_Info.runflag.error_app_size == 1)
        return true;
    else
        return false;
}


//========================SHELL================================================
static bool_t ibootinfo( )
{
    printf("iboot version:V%2d.%2d.%2d \n\r",Iboot_App_Info.iboot_ver_small,Iboot_App_Info.iboot_ver_medium,
                                                                        Iboot_App_Info.iboot_ver_large);
    printf("iboot build time : %4d/%2d/%2d %2d:%2d:%2d \n\r",Iboot_App_Info.iboot_build_year,\
            Iboot_App_Info.iboot_build_mon, Iboot_App_Info.iboot_build_day, Iboot_App_Info.iboot_build_hour, \
            Iboot_App_Info.iboot_build_min, Iboot_App_Info.iboot_build_sec);
//    printf("Iboot start addr: 0x%8llx \n\r", Iboot_App_Info.ibootstartaddr);
    printf("board name : %s \n\r", Iboot_App_Info.board_name);

    return true;
}


static bool_t appinfo( )
{
    struct AppHead*  p_apphead = gc_pAppOffset;
    if(p_apphead->djy_flag[0]!='d' || p_apphead->djy_flag[1]!='j' || p_apphead->djy_flag[2]!='y' )
    {
        printf("cand find APP \r\n");
        return true;
    }
    printf("app ver :v%d \r\n",p_apphead->app_ver);
    printf("app size: %f k \r\n",(float)p_apphead->file_size/1024);
    if(p_apphead->app_name[0] == 0)
        printf("app name: NULL  \r\n");
    else
        printf("app name: %s  \r\n",p_apphead->app_name);
    printf("app appheadsize: %d  \r\n",p_apphead->app_head_size);

    return true;
}
//==============================================================================
//功能：设置更新后运行指定模式
//参数：0 -- 运行iboot；1 -- 运行app
//返回值：true
//==============================================================================
//原名：Iboot_SetUpdateRunModet
bool_t Iboot_SetAfterUpdateRunMode(u8 mode)
{
    Iboot_App_Info.runflag.after_update_runmode = mode;

    return true;
}
//==============================================================================
//功能：获取在完成程序升级后，指定的运行模式
//参数：无
//返回值：0 -- 运行iboot；1 -- 运行app
//==============================================================================
char Iboot_GetAfterUpdateRunModet(void)
{
    return Iboot_App_Info.runflag.after_update_runmode;
}
#endif
//==============================================================================
//功能：获取iboot和APP的信息
//参数：共享信息结构体地址
//返回值：无
//==============================================================================
void Iboot_GetAppInfo(struct IbootAppInfo *get_info)
{
    struct IbootAppInfo *info = get_info;

    memcpy((char *)info, (const char *)&Iboot_App_Info, sizeof(struct IbootAppInfo));
}
//==============================================================================
//功能：获取上电复位标志
//参数：无
//返回值：true -- 上电复位；false -- 非上电复位
//==============================================================================
bool_t Iboot_GetPowerOnResentFlag(void)
{
    if(Iboot_App_Info.runflag.power_on_reset_flag == 1)
        return true;
    else
        return false;
}

static bool_t Iboot_IAP_Mode( )
{
#if (CFG_RUNMODE_BAREAPP == 0)
    if(Iboot_App_Info.runflag.hard_set_run_iboot)
        printf( "硬件件设置运行iboot\n\r");
    if(Iboot_App_Info.runflag.restart_run_iboot)
        printf( "指示启动后运行Iboot   \r\n");
    if(Iboot_App_Info.runflag.restart_run_app)
        printf( "指示启动后运行APP    \r\n");
    if(Iboot_App_Info.runflag.runmode_iboot)
        printf( "当前运行模式是iboot  \r\n");
    if(Iboot_App_Info.runflag.runmode_app  )
        printf( "当前运行模式为app  \r\n");
    if(Iboot_App_Info.runflag.Before_run_iboot)
        printf( "之前运行模式为iboot \r\n");
    if(Iboot_App_Info.runflag.Before_run_app  )
        printf( "之前运行模式为app  \r\n");
    if(Iboot_App_Info.runflag.run_app_form_file)
        printf( "运行模式：从文件中加载app  \r\n");
    if(Iboot_App_Info.runflag.run_iboot_and_update_app)
        printf( "启动（Iboot）后，自动升级APP    \r\n");
    if(Iboot_App_Info.runflag.run_app_and_update_iboot)
        printf( "启iboot后升级iboot自身          \r\n");
    if(Iboot_App_Info.runflag.update_from)
        printf( "升级文件来源    %d   \r\n", Iboot_App_Info.runflag.update_from); //升级文件来源（默认为0不打印） 1——3待定义
    if(Iboot_App_Info.runflag.after_update_runmode        )
        printf( "升级完成后运行  app \r\n");
    else
        printf( "升级完成后运行  iboot \r\n");
    if(Iboot_App_Info.runflag.error_app_check       )
        printf( "校验出错     \r\n");
    if(Iboot_App_Info.runflag.error_app_no_file     )
        printf( "没有app文件或文件格式错误      \r\n");
    if(Iboot_App_Info.runflag.error_app_size  )
        printf( "App文件大小错误   \r\n");
#endif
     //上电复位硬件标志0=无此硬件；1=有此硬件，但无标志；2=有标志，阅后即焚；3=有，非阅后即焚；
    if(Iboot_App_Info.runflag.hard_wdt_reset)
        printf( "看门狗复位 \r\n");
    //看门狗复位标志
    if(Iboot_App_Info.runflag.soft_reset_flag)
        printf( "软件引起的内部复位 \r\n");
    if(Iboot_App_Info.runflag.reboot_flag)
        printf( "reboot 标志 \r\n");
    if(Iboot_App_Info.runflag.hard_reset_flag)
        printf( "外部硬件复位标志 \r\n");

    if(Iboot_App_Info.runflag.low_power_wakeup)
        printf( "低功耗深度休眠中断唤醒标志 \r\n");
    if(Iboot_App_Info.runflag.call_fun_reset)
        printf( "本次复位/重启是主动调用相关函数引发的\r\n");
    else
        printf( "本次复位/重启是异常重启 \r\n");
    if(Iboot_App_Info.runflag.power_on_reset_flag )
        printf( "上电复位标志 \r\n");

    return true;
}
#if (CFG_RUNMODE_BAREAPP == 0)
ADD_TO_ROUTINE_SHELL(ibootinfo,ibootinfo,NULL);
ADD_TO_ROUTINE_SHELL(appinfo,appinfo,NULL);
//ADD_TO_ROUTINE_SHELL(filesource,Iboot_SetUpdateSource,NULL);
#endif      //for (CFG_RUNMODE_BAREAPP == 0)
ADD_TO_ROUTINE_SHELL(iapmode,Iboot_IAP_Mode,NULL);

