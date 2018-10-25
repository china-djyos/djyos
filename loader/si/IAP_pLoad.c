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
//����ģ��: Iboot
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ��Ӧ�ñ�̵ĺ����ļ�
//����˵��:
//�޶���ʷ:
//1. ����: 2016-06-17
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stdio.h"
//#include "driver.h"
#include "IAP.h"
#include "djyos.h"
//#include "verify.h"
#include <stdlib.h>
#include "string.h"
#include "cpu_peri.h"
#include "cpu-optional.h"
#include "IAP_Inner.h"
#include <version.h>
#include <board-config.h>
#include "dbug.h"

#include "component_config_loader.h"
#if(CN_CPU_OPTIONAL_CACHE==1)
#include "set-cache.h"
#endif
#include "dbug.h"

extern struct IbootCtrl *gc_ptIbootCtrl;
extern struct AppInfo   *gc_ptAppInfo;
extern const u8 g_IbootCRC;
extern u32 *gc_AppAddr;
#define  CN_IBOOT_COMPILE_DATE     __DATE__
#define  CN_IBOOT_COMPILE_TIME     __TIME__
tagIapVar pg_IapVar __attribute__ ((section(".IapBootFlgRam")));
const char updateflag[] = "update"; // ҪŪ��const�����Ǿֲ��������������������ROM
const char bootflag[] = "RunIboot"; // ҪŪ��const�����Ǿֲ��������������������ROM

extern void Load_Preload(void);
extern bool_t IAP_IsForceIboot(void);
extern void (*__AppStart)(void); // ������iboot.lds�ж�λ�ĺ�������APP��lds�ļ��У�ȷ������ָ�� AppStart ����
//extern void __AppStart(void);  // �ɰ汾


static s32 __IBOOT_InitMessage(void);
static s32 __IBOOT_PassMessage(void);
static s32 __IBOOT_Actions(u8 bArgc, ...);

extern void _asm_bl_fun(u32 fun_addr);

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t HAL_SetUpdateFlag(void)
{
    pg_IapVar.UpdateFlag[0]=0x75;
    pg_IapVar.UpdateFlag[1]=0x70;
    pg_IapVar.UpdateFlag[2]=0x64;
    pg_IapVar.UpdateFlag[3]=0x61;
    pg_IapVar.UpdateFlag[4]=0x74;
    pg_IapVar.UpdateFlag[5]=0x65;
    pg_IapVar.UpdateFlag[6]=0x0;
    return true;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t HAL_ClrUpdateFlag(void)
{
    pg_IapVar.UpdateFlag[0]=0x64;
    pg_IapVar.UpdateFlag[1]=0x64;
    pg_IapVar.UpdateFlag[2]=0x64;
    pg_IapVar.UpdateFlag[3]=0x64;
    pg_IapVar.UpdateFlag[4]=0x64;
    pg_IapVar.UpdateFlag[5]=0x64;
    pg_IapVar.UpdateFlag[6]=0x64;
    return true;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
bool_t HAL_CheckUpdateFlag(void)
{
    u8 i;
    for(i=0;i<7;i++)
    {
        if(pg_IapVar.UpdateFlag[i]!=updateflag[i])
        {
            return false;
        }
    }
    return true;
}

static const u32 g_CRC32_tab[] = {
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static u32 __IAP_crc32( char *buf, int len)
 {
     u32 ret = 0xFFFFFFFF;
     int   i;
     for(i = 0; i < len;i++)
     {
          ret = g_CRC32_tab[(ret ^buf[i] ) &0xFF] ^ (ret >> 8);
     }
      ret = ~ret;
     return ret;
 }

//-----------------------------------------------------------------
//���ܣ���ȡAPP�ڴ洢�����׵�ַ
//��������
//���أ�32λ�ĵ�ַ.
//------------------------------------------------------------------
static u32 __IAP_GetAPPStartAddr(void)
{
     return (u32)(&gc_ptAppInfo->RomStartAddr);
}

//-----------------------------------------------------------------
//���ܣ���ȡAPP�Ĵ�С(�ֽ���)
//��������
//���أ�APP bin�ļ��Ĵ�С.
//------------------------------------------------------------------
 static u32 __IAP_GetAPPSize(void)
{
    return (gc_ptIbootCtrl->AppSize);
}

//-----------------------------------------------------------------
//���ܣ�������Iboot����APP���豸����APP����Iboot������ͨ��shell�л����л��ķ�
//      ���ǣ����ڴ��ض�λ������־��Ȼ��������
//��������
//���أ��ޡ�
//˵����TRUE = run APP��false = run Iboot��
//-----------------------------------------------------------------
 bool_t IAP_IsRamIbootFlag(void)
 {
    u8 i;
    for(i=0;i<8;i++)
    {
        if(pg_IapVar.IbootFlag[i]!=bootflag[i])
        {
            return false;
        }
    }
    return true;
 }

//----ѡ�������Ŀ����-----------------------------------------------------------
//���ܣ�ѡ����ش洢�洢�����ĸ���Ŀ����,Iboot��APP��
//����: �ޡ�
//����: �ޡ�
//----------------------------------------------------------------------------
void IAP_SelectLoadProgam(void)
{
    bool_t result=true;
    u32 crc,len;
    u32 addr;
    pg_IapVar.IbootStatus=EN_NO_ERR;

    __IBOOT_InitMessage();

    result = IAP_IsForceIboot();
    if(result)
    {
        pg_IapVar.IbootStatus=EN_FORCE_IBOOT;
        pg_IapVar.RunMode = CN_IAP_MODE_IBOOT;
        Load_Preload();   //����Iboot
    }
    else
    {
        result=IAP_IsRamIbootFlag();
        if(result)
        {
            pg_IapVar.IbootStatus=EN_RAM_IBOOT_FLAG;
            pg_IapVar.RunMode = CN_IAP_MODE_IBOOT;
            Load_Preload();   //����Iboot
        }

        // add by zhb 20170803
        // check if need update app
        extern bool_t HAL_CheckUpdateFlag(void);
        result=HAL_CheckUpdateFlag();
        if(result)
        {
               pg_IapVar.IbootStatus=EN_APP_UPDATE_MODE;
               pg_IapVar.RunMode = CN_IAP_MODE_IBOOT;
               Load_Preload();   //����Iboot
        }

         //���������APP��ΪDirectRunģʽ,��
#if (CFG_APP_RUNMODE == EN_DIRECT_RUN)
        {
             //gc_AppAddr�ĵ�ַ��lds��ֱ�Ӹ�ֵ��
             //RomStartAddr����app.bin�е�
             //���iboot��memory.lds�к�APP��memory.lds��IbootSize���岻һ�£�
             //��ֱ������Iboot��������Iboot������shell�������Ӧ��Ϣ
            if((u32)(gc_AppAddr)==gc_ptAppInfo->RomStartAddr)
            {
                if(gc_ptIbootCtrl->flag==CN_APP_CTRL_APP_FLAG)
                {
                    //len��IAP���غ�д��ġ�
                    //AppSize�ǰ�����APP.bin�е�
                    //��������ļ���������if����������ǿ������Iboot
                    len=__IAP_GetAPPSize();
                    if(gc_ptAppInfo->AppSize==len)
                    {
                        //��Щ�߿ɿ����ر��Ǻ���Ӧ���У���ҪУ��flash�еĴ���
                        //�Ƿ񱻴۸ġ���Ҫ��Ϊflash���԰�ȫ�����������޴����ڡ�
                        //���ǣ�У����Ҫ���ĺܶ�ʱ�䣬������Ҫ����������Ӧ�ã�
                        //���߿ɿ���Ҫ����ô�ߵ�Ӧ�ã�������Iboot_Config.c
                        //�йر�CRCУ�顣
#if(CFG_IBOOT_CRC==EN_USE_APP_CRC)   //����Ϊ��ҪУ��
                        {
                            addr=__IAP_GetAPPStartAddr();
                            char *buf;
                            buf=(char *)addr;
                            crc=__IAP_crc32(buf,len);
                            if(crc==gc_ptIbootCtrl->Iap_crc)
                            {
                                pg_IapVar.RunMode = CN_IAP_MODE_APP;
                                __IBOOT_PassMessage();
                                __asm_bl_fun(*__AppStart);
                                // (*__AppStart)(); // ֱ����ת���������⣨������������
                            }
                            else
                            {
                                pg_IapVar.IbootStatus=EN_CRC_ERR;
                                pg_IapVar.RunMode = CN_IAP_MODE_IBOOT;
                                Load_Preload();
                            }
                        }
#else       //for (CFG_IBOOT_CRC==EN_USE_APP_CRC)
                        {
                            pg_IapVar.RunMode = CN_IAP_MODE_APP;
                            __IBOOT_PassMessage();
                            __asm_bl_fun(*__AppStart);
                            // (*__AppStart)(); // ֱ����ת���������⣨������������
                        }
#endif      //for (CFG_IBOOT_CRC==EN_USE_APP_CRC)
                    }
                    else
                    {
                        pg_IapVar.IbootStatus = EN_BIN_INCOMPLETED_ERR;
                        pg_IapVar.RunMode = CN_IAP_MODE_IBOOT;
                        Load_Preload();
                    }
                }
                else if(gc_ptIbootCtrl->flag==CN_APP_CTRL_DBG_FLAG)
                {
                    pg_IapVar.RunMode = CN_IAP_MODE_APP;
                    __IBOOT_PassMessage();
                    __asm_bl_fun(*__AppStart);
                    // (*__AppStart)(); // ֱ����ת���������⣨������������
                }
                else
                {
                    pg_IapVar.IbootStatus=EN_APP_FLAG_ERR;
                    pg_IapVar.RunMode = CN_IAP_MODE_IBOOT;
                    Load_Preload();
                }
            }
            else
            {
                if( (gc_ptAppInfo->RomStartAddr == 0) ||
                        (gc_ptAppInfo->RomStartAddr == 0xFFFFFFFF))
                    pg_IapVar.IbootStatus=EN_FILE_NO_EXSIT_ERR;
                else
                    pg_IapVar.IbootStatus=EN_lDS_MISMATCH;
                pg_IapVar.RunMode = CN_IAP_MODE_IBOOT;
                Load_Preload();
            }
        }
#else       //for (CFG_IBOOT_TYPE == EN_DIRECT_RUN)
        {
            pg_IapVar.IbootStatus=EN_LOAD_FROM_DATA_MODE;
            pg_IapVar.RunMode = CN_IAP_MODE_IBOOT;
            Load_Preload();
        }
#endif      //for (CFG_IBOOT_TYPE == EN_DIRECT_RUN)
    }
}

// ============================================================================
// ���ܣ���ȡ������ʽ
// ������
// ���أ�������ʽ
// ��ע��
// ============================================================================
u32 IAP_GetMethod(void)
{
    return (pg_IapVar.message.u.a2i.update.dwMethod);
}

// ============================================================================
// ���ܣ�����������ʽ
// ������dwMethod -- ������ʽ��(1:ͨ���ļ�ϵͳ)
//                          ��2��IAP�ļ�ϵͳ��·��Ҫ����Ϊ�豸����
// ���أ�
// ��ע��
// ============================================================================
s32 IAP_SetMethod(u32 dwMethod)
{
    pg_IapVar.message.u.a2i.update.dwMethod = dwMethod;
    return (0);
}

// ============================================================================
// ���ܣ���ȡ�����ļ����ڵ�·��
// ������pPath -- �ļ�����·��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
char *IAP_GetPath(void)
{
    return (pg_IapVar.message.u.a2i.update.path);
}

// ============================================================================
// ���ܣ����������ļ����ڵ�·��
// ������pPath -- �ļ�����·��
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
s32 IAP_SetPath(char *pPath)
{
    if(!pPath)
        return (-1);

    if(strlen(pPath) > (sizeof(pg_IapVar.message.u.a2i.update.path)-1))
    {
        debug_printf("IAP","cannot set too long path, <max:%d>",
                        (sizeof(pg_IapVar.message.u.a2i.update.path)-1));
        return (-1);
    }

    strcpy(pg_IapVar.message.u.a2i.update.path, pPath);

    return (0);
}

// ============================================================================
// ���ܣ�IBOOTģʽ�£���ӡIBOOT��Ϣ
// ������
// ���أ��ɹ���0����
// ��ע��
// ============================================================================
static s32 __IBOOT_PrintIbootVersion(void)
{
    debug_printf("IAP","IBOOT VERSION : %s - %s - %s, %s", CFG_IBOOT_VERSION,
                DJY_BOARD, CN_IBOOT_COMPILE_DATE, CN_IBOOT_COMPILE_TIME);
    debug_printf("IAP","\r\n");

    return (0);
}

// ============================================================================
// ���ܣ�IBOOTģʽ��messageִ�ж���
// ������
// ���أ��ɹ���0����
// ��ע����ǰֻ��һ��ִ�ж�����û�н����������
// ============================================================================
static s32 __IBOOT_Actions(u8 bArgc, ...)
{
    __IBOOT_PrintIbootVersion();
    return (0);
}

// ============================================================================
// ���ܣ�ȱʡ��ӡIBOOT��Ϣ
// ������
// ���أ��ɹ���0����
// ��ע��
// ============================================================================
s32 __DEFAULT_PrintIbootVersion(void)
{
    debug_printf("IAP","IBOOT VERSION : %s - %s - %s, %s",
                        pg_IapVar.message.u.i2a.version.pIBOOT,
                        pg_IapVar.message.u.i2a.version.pBOARD,
                        pg_IapVar.message.u.i2a.version.pIBOOT_COMPILE_DATE,
                        pg_IapVar.message.u.i2a.version.pIBOOT_COMPILE_TIME);
    debug_printf("IAP","\r\n");

    return (0);
}

 // ============================================================================
 // ���ܣ���ӡIBOOT�İ汾
 // ������
 // ���أ��ɹ���0����ʧ�ܣ�-1����
 // ��ע��
 // ============================================================================
s32 IAP_PrintIbootVersion(void)
{
    s32 res = 0;

    if(pg_IapVar.message.actions)
    {
        res = pg_IapVar.message.actions(1, 1);
    }
    else
    {
        res = __DEFAULT_PrintIbootVersion();
    }

    return (res);
}


bool_t IAP_GetIbootVersion(tagMessage *IbootVer)
{
    if(IbootVer==NULL)
        return false;
    IbootVer->u.i2a.version.pIBOOT=pg_IapVar.message.u.i2a.version.pIBOOT;
    IbootVer->u.i2a.version.pBOARD=pg_IapVar.message.u.i2a.version.pBOARD;
    IbootVer->u.i2a.version.pIBOOT_COMPILE_DATE=pg_IapVar.message.u.i2a.version.pIBOOT_COMPILE_DATE;
    IbootVer->u.i2a.version.pIBOOT_COMPILE_TIME=pg_IapVar.message.u.i2a.version.pIBOOT_COMPILE_TIME;

    return true;
}
// ============================================================================
// ���ܣ���IBOOT�汾��Ϣ���ݸ�APP
// ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 __IBOOT_PassMessage(void)
{
    pg_IapVar.message.u.i2a.version.pIBOOT = CFG_IBOOT_VERSION;
    pg_IapVar.message.u.i2a.version.pIBOOT_COMPILE_DATE = CN_IBOOT_COMPILE_DATE;
    pg_IapVar.message.u.i2a.version.pIBOOT_COMPILE_TIME = CN_IBOOT_COMPILE_TIME;
    pg_IapVar.message.u.i2a.version.pBOARD = DJY_BOARD;
    pg_IapVar.message.actions = NULL;
    return (0);
}

// ============================================================================
// ���ܣ���ʼ����Ϣ����
// ������
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
// ============================================================================
static s32 __IBOOT_InitMessage(void)
{
    pg_IapVar.message.actions = __IBOOT_Actions;
    return (0);
}
