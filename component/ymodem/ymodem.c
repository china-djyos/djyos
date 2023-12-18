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
//����ģ��:ymodemģ��
//����:  ����.
//�汾��V2.0.0
//�ļ�����:ymodemģ�飬����ymodemЭ���ϴ��������ļ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2013-10-06
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <device.h>
#include "stat.h"

#include "os.h"
#include "shell.h"
#include <misc/misc.h>
#include <misc/crc.h>
#include "dbug.h"
#include "ymodem.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_Ymodem(void);
//    ModuleInstall_Ymodem();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"ymodem"       //ymodem
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","file system"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_YMODEM == false )
//#warning  " ymodem  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_YMODEM    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
#define     CFG_YMODEM_BUF_NUM      32              //"����ymodem������",ÿ��1024�ֽ�
//%$#@num,1,100000000,
#define     CFG_YMODEM_PKG_TIMEOUT  (15*1000*1000)  //"�������ʱʱ��",΢��
#define     CFG_YMODEM_TIMEOUT      (300) //"ymodem�����ܳ�ʱʱ��",��
//%$#@enum,true,false,
//%$#@string,1,128,
#define     CFG_YMODEM_DEVNAME      "std"           //"ymodem�����豸��std��ʾʹ�ñ�׼��������豸"
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


static s32 write_char(s32 ch, int fd)
{
    ssize_t res = write(fd, (u8*)&ch, 1);
    if (res==1) return ch;
    return EOF;
}

static s32 read_char(int fd)
{
    u8 ch=0;
    ssize_t res = read(fd, &ch, 1);
    if (res == 1) return ch;
    return EOF;
}


#define CN_YMODEM_PKGBUF_SIZE           (1029)
#define CN_YMODEM_NAME_LENGTH           (256)

// YMODEM����
#define CN_YMODEM_SOH     1         //128�ֽ���ʼ��
#define CN_YMODEM_STX     2         //1028�ֽ���ʼ��
#define CN_YMODEM_EOT     4
#define CN_YMODEM_ACK     6
#define CN_YMODEM_NAK     0x15
#define CN_YMODEM_C       0x43          //��д��ĸC
#define CN_YMODEM_CAN     0x18

// YMODEM���ݰ�����
#define CN_YMODEM_SOH_SIZE  128        //SOH���ݰ���СΪ128�ֽ�
#define CN_YMODEM_STX_SIZE  1024       //STX���ݰ���СΪ1024

typedef enum
{
     YMODEM_OK= 0,              //��������
     YMODEM_PARAM_ERR,          //��������
     YMODEM_FILE_ERR,           //�ļ���������
     YMODEM_TIMEOUT,            //��ʱ����
     YMODEM_MEM_ERR,            //�ڴ治��
     YMODEM_CAN_TRANS,          //ȡ������
     YMODEM_MEDIA_ERR,          //Ӳ������
     YMODEM_UNKNOW_ERR
}YMRESULT;

typedef enum
{
    ENUM_YMODEM_STA_INFO = 0x80,        //YMODEM��Ϣ��
    ENUM_YMODEM_STA_SOH,                //128�ֽڵ����ݰ�
    ENUM_YMODEM_STA_STX,                //1024�ֽڵ����ݰ�
    ENUM_YMODEM_STA_EOT,                //������
    ENUM_YMODEM_STA_ZERO,               //ȫZERO��
}YMSTATUS;

typedef enum
{
    YMODEM_FILE_NOOPS= 0xF0,            //���ļ�����
    YMODEM_FILE_OPENW ,                 //���ļ�����д
    YMODEM_FILE_OPENR ,                 //���ļ����ڶ�
    YMODEM_FILE_WRITE,                  //д�ļ����
    YMODEM_FILE_READ,                   //���ļ����
    YMODEM_FILE_STAT,
    YMODEM_FILE_CLOSE,                  //�ر��ļ����
}YMFILEOPS;


typedef struct __Ymodem
{
    FILE    *File;                      //file ptr for file op
//  const char    *Path;                //�ļ����ڵ�·����δ���ļ�����
    char    *FileName;                  //�ļ�����
    struct MutexLCB *pYmodemMutex;      //�������
//  YMFILEOPS FileOps;                  //�ļ�����
    u32     FileSize;                   //��ǰ�����ļ���С
    u32     FileCnt;                    //д���ļ���С����
    u32     PkgNo;                      //����
    u32     PkgSize;                    //��ǰ������
    s64     StartTime;                  //start time
    u32     TimeOut;                    //time out
    YMSTATUS Status;                    //ymodem status
    u8      *FileBuf;                   //file buf
    u32     FileBufCnt;                 //�ļ���������С����
    u8      *PkgBuf;
    u32     PkgBufCnt;                  //PKG��С����
}tagYmodem;

bool_t uploadym(char *Param);
bool_t downloadym(char *Param);

static tagYmodem *pYmodem = NULL;
static s32 s_s32gYmodemDevIn,s_s32gYmodemDevOut;

//----ymodemģ�ͳ�ʼ��---------------------------------------
//����: ymodem�����ļ�ģ��ӿں�������module_trim�е���
//����: ��
//����: �����򷵻�false
//-----------------------------------------------------------
bool_t ModuleInstall_Ymodem(void)
{
    if(strcmp(CFG_YMODEM_DEVNAME, "std") == 0)
    {
        s_s32gYmodemDevIn = STDIN_FILENO;
        s_s32gYmodemDevOut = STDOUT_FILENO;
    }
    else
    {
        s_s32gYmodemDevIn = s_s32gYmodemDevOut = open(CFG_YMODEM_DEVNAME, O_RDWR);
        if(s_s32gYmodemDevIn == -1)
            return false;
    }
    pYmodem = (tagYmodem *)malloc(sizeof(tagYmodem));
    if(NULL != pYmodem)
    {
        pYmodem->FileName = (char *)malloc(CN_YMODEM_NAME_LENGTH);
        if(NULL == pYmodem->FileName)
        {
            free(pYmodem);
            return false;
        }
        strcpy(pYmodem->FileName, "/xip-app/");
        pYmodem->pYmodemMutex = Lock_MutexCreate("YMODEM_MUTEX");
        if(NULL != pYmodem->pYmodemMutex)
        {
//          pYmodem->Path = NULL;


                return true;
        }
        else
        {
            free(pYmodem->FileName);
            free(pYmodem);
        }
    }

    error_printf("MODULE","YMODEM FAILED !\r\n");
    return false;
}

//----��ȡ��������-------------------------------------------
//���ܣ�������ͨ������һ������
//������
//���أ�
//��ע�����������������Ʊ���������߼����䣬��û�ж���len����ʱ��
//     ��һֱ�ȴ�timeout
//-----------------------------------------------------------
static u32 __Ymodem_Gets(u8 *buf,u32 len)
{
    u32 res, bytesToGet = len;
    s64 end, log;

    log = DJY_GetSysTime();
    end = log + (s64)CFG_YMODEM_PKG_TIMEOUT;

    while(1)
    {
        res = read(s_s32gYmodemDevIn, buf, bytesToGet);
        bytesToGet -= res;
        if(!bytesToGet)
            break;

        if(DJY_GetSysTime() > end)
            break;

        buf += res;
    }

    return (len - bytesToGet);
}

// ============================================================================
// ���ܣ�����ymodem�����ļ��ľ���·�����ú���ѡ���Ե��ã�
//      1.��Ӧ����Ҫ�������·�����򲻵��øú�����ͨ��������ʽ����shell���޸�·��;
//      2.���øú�����ͨ��Ymodem���ص�·���򲻿ɱ任;
//      3.����˵����������iboot����������·������䣬���Path = "/iboot"
// ������Path��ͨ��Ymodem��ŵľ���·��
// ���أ�true,�ɹ�������ʧ��
// ============================================================================
bool_t Ymodem_PathSet(const char *Path)
{
    bool_t Ret = false;
    u32 len;

    if( NULL != Path )
    {
        len = strlen(Path);
        // -2����ΪPath���һ���ַ�����'/'����Ҫ����
        if(len < CN_YMODEM_NAME_LENGTH - 2)
        {
            if(Lock_MutexPend(pYmodem->pYmodemMutex,CN_TIMEOUT_FOREVER))
            {
        //      pYmodem->Path = Path;
                strcpy(pYmodem->FileName, Path);
                //���һ���ַ�����'/'��'\\'����Ҫ����
                if((pYmodem->FileName[len-1] != '/') && (pYmodem->FileName[len-1] != '\\'))
                {
                    pYmodem->FileName[len] = '/';
                    pYmodem->FileName[len] = '\0';
                }
                Lock_MutexPost(pYmodem->pYmodemMutex);
            }
            Ret = true;
        }
    }
    return Ret;
}

//----��ȡ����----------------------------------------------
//����: ��buf����ȡһ���ɿո���н����������ĵ��ʣ�next����
//      ������һ�������׵�ַ�����û����һ�����ʣ���next=NULL��
//����: buf�����������ַ���
//      next��������һ������ָ��
//����: ��ȡ�ĵ���ָ�룬�ѽ����ʺ���ķָ������ɴ�������'\0'
//-----------------------------------------------------------
//todo:shell.c��Sh_GetWord������˺�����ͬ��
static char *__Ymodem_GetWord(char *buf,char **next)
{
    uint32_t i=0;
    *next = NULL;
    if(buf == NULL)
        return NULL;
    while(1)
    {
        if((buf[i] == ' ') || (buf[i] == 0))
        {
            buf[i] = '\0';
            break;
        }
        i++;
    }
    i++;
    while(buf[i] != 0)
    {
        if(buf[i]!=' ')
        {
            if((buf[i] == '\n') || (buf[i] == '\r'))
                *next = NULL;
            else
                *next = &buf[i];
            break;
        }
        i++;
    }
    return buf;
}
//------ymodemȡ������---------------------------------------
//���ܣ�ymodemģ��֪ͨ�Զ�ȡ�����δ���
//��������
//���أ���
//-----------------------------------------------------------
static void __Ymodem_CancelTrans(void)
{
    write_char(CN_YMODEM_CAN, s_s32gYmodemDevOut);
    write_char(CN_YMODEM_CAN, s_s32gYmodemDevOut);
    write_char(CN_YMODEM_CAN, s_s32gYmodemDevOut);
}

//------У��ymodem���ݰ�-------------------------------------
//���ܣ�ymodemģ��У����յ������ݰ�
//������buf�����ݰ�����
//      pack_len: ymodem���ݰ�����
//���أ�true:  ���ݰ�У����ȷ
//      false: ���ݰ�У�����
//----------------------------------------------------------
static bool_t __Ymodem_PackCheck(u8* buf, u32 pack_len)
{
    u16 checksum,check;
    if((buf[1] + buf[2]) != 0xff)               //У�����������
    {
        write_char(CN_YMODEM_NAK, s_s32gYmodemDevOut);     //Ӧ��nak�������ط�
        return false;
    }
    checksum = CRC_16(buf+3, pack_len);
    if(CN_CFG_BYTE_ORDER == CN_CFG_LITTLE_ENDIAN)
    {
        check = (buf[pack_len+3]<<8) + buf[pack_len+4];
    }
    else
    {
        check = *(u16 *)&buf[pack_len+3];
    }
    if(checksum != check)    //CRCУ�����
    {
        write_char(CN_YMODEM_NAK, s_s32gYmodemDevOut);                    //Ӧ��nak�������ط�
        return false;
    }

    return true;
}

static void __Ymodem_WriteCrc16(u8 *package, u32 pack_len)
{
    u16 checksum;

    checksum = CRC_16(package+3, pack_len);
    if(CN_CFG_BYTE_ORDER == CN_CFG_LITTLE_ENDIAN)
    {
        package[3+pack_len] = (checksum >> 8) & 0xFF;
        package[3+pack_len + 1] = (checksum) & 0xFF;

    }
    else
    {
        package[3+pack_len] = (checksum) & 0xFF;
        package[3+pack_len + 1] = (checksum >> 8) & 0xFF;
    }
}

// ��ȡ������YMODEM���ݰ����������ݰ���ACK��CAN�Ȱ�
static YMRESULT __Ymodem_GetPkg(tagYmodem *ym)
{
    YMRESULT Ret = YMODEM_OK;
    u32 bytes;

    if(ym->PkgBufCnt == 0)
    {
        if(1 != read(s_s32gYmodemDevIn, ym->PkgBuf, 1))
            Ret = YMODEM_TIMEOUT;
    }

    switch(ym->PkgBuf[0])
    {
    case CN_YMODEM_SOH:
        ym->PkgSize = CN_YMODEM_SOH_SIZE;
        break;
    case CN_YMODEM_STX:
        ym->PkgSize = CN_YMODEM_STX_SIZE;
        break;
    case CN_YMODEM_ACK:
    case CN_YMODEM_NAK:
    case CN_YMODEM_C:
    case CN_YMODEM_CAN:
    case CN_YMODEM_EOT:
        ym->PkgSize = 1;
        break;
    default:
        ym->PkgSize = 1;
        Ret = YMODEM_UNKNOW_ERR;
        break;
    }

    if(ym->PkgSize > 1)
    {
        bytes = __Ymodem_Gets(ym->PkgBuf + 1,ym->PkgSize + 4);
        if(bytes != ym->PkgSize + 4)
            Ret = YMODEM_TIMEOUT;
    }

    return Ret;
}

static bool_t __Ymodem_FilePathMerge(char *name)
{
    u32 PathLen,NameLen;

    if(NULL != name)
    {
        //PathLen = strlen(pYmodem->FileName);
        PathLen = strlen(pYmodem->FileName);
        NameLen = strlen(name);
        if(PathLen + NameLen + 1 < CN_YMODEM_NAME_LENGTH)
        {
            strcpy(pYmodem->FileName+PathLen,name);
            return true;
        }
        else
            return false;
    }
    return false;
}
static bool_t __Ymodem_InfoPkg(tagYmodem *ym)
{
    char *NextParam,*strFileSize,*FileName;

    FileName = __Ymodem_GetWord((char*)&ym->PkgBuf[3],&NextParam);
    strFileSize = __Ymodem_GetWord(NextParam,&NextParam);

    if(__Ymodem_FilePathMerge(FileName))
    {
        ym->FileSize = strtol(strFileSize, (char **)NULL, 0);
//        ym->FileOps = YMODEM_FILE_OPEN;
        ym->Status = CN_YMODEM_SOH;
        ym->PkgNo = 1;
        return true;
    }
    return false;
}

static bool_t __Ymodem_IsZeroPkg(tagYmodem *ym)
{
    u8 i;

    if( (ym->PkgBuf[1] == 0x00) && (ym->PkgBuf[2] == 0xFF) )
    {
        for(i = 0; i < 128; i++)
        {
            if(ym->PkgBuf[3+i] != 0x00)
            {
                return false;
            }
        }
    }

    return true;
}


static YMRESULT __Ymodem_FileOps(tagYmodem *ym, YMFILEOPS cmd)
{
    YMRESULT Ret = YMODEM_OK;
    struct stat FpInfo;
    u32 FileOpsLen;

    //���ļ����в���
    switch(cmd)
    {
    case YMODEM_FILE_OPENW:
        ym->File = fopen(ym->FileName,"w+");        //���ļ����������򴴽�
        write_char(CN_YMODEM_ACK, s_s32gYmodemDevOut);
        write_char(CN_YMODEM_C, s_s32gYmodemDevOut);
        if(ym->File == NULL)
        {
            Ret = YMODEM_FILE_ERR;
        }
        break;
    case YMODEM_FILE_OPENR:
        ym->File = fopen(ym->FileName,"r");         //���ļ�
        if(ym->File == NULL)
        {
            Ret = YMODEM_FILE_ERR;
        }
        break;
    case YMODEM_FILE_WRITE:
        memcpy(ym->FileBuf + ym->FileBufCnt,ym->PkgBuf + 3,ym->PkgSize);
        ym->FileBufCnt += ym->PkgSize;
        if((ym->FileBufCnt >= ym->FileSize - ym->FileCnt) ||
                (ym->FileBufCnt >= CFG_YMODEM_BUF_NUM*1024)) //����㹻��д��flash
        {
            if(ym->FileBufCnt >= ym->FileSize - ym->FileCnt)                                   //�жϱ���д���С
                FileOpsLen = ym->FileSize - ym->FileCnt;
            else
                FileOpsLen = ym->FileBufCnt;
            ym->FileBufCnt = 0;
            if(FileOpsLen != fwrite(ym->FileBuf, 1, FileOpsLen, ym->File))
            {
                Ret = YMODEM_FILE_ERR;
            }
            ym->FileCnt += FileOpsLen;
        }
        write_char(CN_YMODEM_ACK, s_s32gYmodemDevOut);
        break;
    case YMODEM_FILE_READ:
        memset(ym->PkgBuf,0x1A,CN_YMODEM_PKGBUF_SIZE);// the last package need filled by CPMEOF(0x1A) at the end
        FileOpsLen = (ym->FileSize - ym->FileCnt > CN_YMODEM_STX_SIZE) ?
                (CN_YMODEM_STX_SIZE) : ym->FileSize - ym->FileCnt;
        ym->PkgSize = CN_YMODEM_STX_SIZE;
        ym->PkgBuf[0] = CN_YMODEM_STX;
        ym->PkgBuf[1] = ym->PkgNo;
        ym->PkgBuf[2] = 0xFF - ym->PkgBuf[1];

        if(FileOpsLen != fread(ym->PkgBuf + 3,1,FileOpsLen,ym->File))
        {
            Ret = YMODEM_FILE_ERR;
        }
        __Ymodem_WriteCrc16(ym->PkgBuf,ym->PkgSize);
        ym->FileCnt += ym->PkgSize;
        break;
    case YMODEM_FILE_STAT:
        if(0 == stat(ym->FileName,&FpInfo))
        {
            ym->FileSize = (u32)FpInfo.st_size;
        }
        else
        {
            Ret = YMODEM_FILE_ERR;
        }
        break;
    case YMODEM_FILE_CLOSE:
        fclose(ym->File);
        break;
    default:
        break;
    }
//  ym->FileOps = YMODEM_FILE_NOOPS;
    return Ret;
}

static YMRESULT __Ymodem_ReceiveProcess(tagYmodem *ym)
{
    YMRESULT Ret = YMODEM_OK;
    s64 CurrentTime;

    while(1)
    {
        Ret = __Ymodem_GetPkg(ym);
        if(Ret != YMODEM_OK)
        {
            __Ymodem_CancelTrans();
            break;
        }
        CurrentTime = DJY_GetSysTime();                  //�ܳ�ʱ����
        if(CurrentTime - ym->StartTime >= ym->TimeOut)
        {
            Ret = YMODEM_TIMEOUT;
            __Ymodem_CancelTrans();
            break;
        }
        //���ݰ�����
        switch(ym->PkgBuf[0])
        {
        case CN_YMODEM_SOH:
            if(!__Ymodem_PackCheck(ym->PkgBuf,ym->PkgSize)) //need retry load
                break;
            if(ym->Status == ENUM_YMODEM_STA_INFO)          //info pkg
            {
                __Ymodem_InfoPkg(ym);
                Ret = __Ymodem_FileOps(ym, YMODEM_FILE_OPENW);
            }
            else if(ym->Status == CN_YMODEM_EOT)
            {
                if(__Ymodem_IsZeroPkg(ym))
                {
                    write_char(CN_YMODEM_ACK, s_s32gYmodemDevOut);//ȫ��������д������
                    goto YMODEM_RECVEXIT;
                }
                else
                {
                    __Ymodem_InfoPkg(ym);       //��������һ���ļ�
                    Ret = __Ymodem_FileOps(ym, YMODEM_FILE_OPENW);
                }
            }
            else                            //�յ�128�ֽڴ�С�����ݰ�
            {
                if(ym->PkgBuf[1] == (ym->PkgNo & 0xff))
                {
                    ym->PkgNo ++;
                    Ret = __Ymodem_FileOps(ym,YMODEM_FILE_WRITE);
                }
                else
                {
                    write_char(CN_YMODEM_NAK, s_s32gYmodemDevOut);    //���Ŵ������ش�
                }
                ym->Status = CN_YMODEM_SOH;
            }
            break;
        case CN_YMODEM_STX:
            if(!__Ymodem_PackCheck(ym->PkgBuf,ym->PkgSize)) //need retry load
                break;
            if(ym->PkgBuf[1] == (ym->PkgNo & 0xff))
            {
                ym->PkgNo ++;
                Ret = __Ymodem_FileOps(ym,YMODEM_FILE_WRITE);
            }
            else
            {
                write_char(CN_YMODEM_NAK, s_s32gYmodemDevOut);//���Ŵ������ش�
            }
            ym->Status = CN_YMODEM_STX;
            break;
        case CN_YMODEM_EOT:
            if( (ym->Status == CN_YMODEM_SOH) || (ym->Status == CN_YMODEM_STX)) //  ��һ��EOT
            {
                write_char(CN_YMODEM_NAK, s_s32gYmodemDevOut); //���յ����������ظ�ACK
                ym->Status = CN_YMODEM_EOT;
            }
            else if(ym->Status == CN_YMODEM_EOT)
            {
                write_char(CN_YMODEM_ACK, s_s32gYmodemDevOut);                         //���յ����������ظ�ACK
                write_char(CN_YMODEM_C, s_s32gYmodemDevOut);                           //���յ����������ظ�C
            }
            break;
        case CN_YMODEM_CAN:
            Ret = YMODEM_CAN_TRANS;
            goto YMODEM_RECVEXIT;
            break;
        default:
            Ret = YMODEM_UNKNOW_ERR;
            break;
        }
        ym->PkgBufCnt = 0;

        if(Ret != YMODEM_OK)            //������� 1.case 2.file ops
        {
            __Ymodem_CancelTrans();
            break;
        }
    }
YMODEM_RECVEXIT:
    __Ymodem_FileOps(ym,YMODEM_FILE_CLOSE);                 //close file
    return Ret;
}

// ��secureCRTʹ��ymodemʱ������������������ݡ�
// ��ʱֻ�ܽ���һ���ļ�
static YMRESULT __Ymodem_ReceiveProcess_CRT(tagYmodem *ym)
{
    YMRESULT Ret = YMODEM_OK;
    s64 CurrentTime;

    while(1)
    {
        Ret = __Ymodem_GetPkg(ym);
        if(Ret != YMODEM_OK)
        {
            __Ymodem_CancelTrans();
            break;
        }
        CurrentTime = DJY_GetSysTime();                  //�ܳ�ʱ����
        if(CurrentTime - ym->StartTime >= ym->TimeOut)
        {
            Ret = YMODEM_TIMEOUT;
            __Ymodem_CancelTrans();
            break;
        }
        //���ݰ�����
        switch(ym->PkgBuf[0])
        {
        case CN_YMODEM_SOH:
            if(!__Ymodem_PackCheck(ym->PkgBuf,ym->PkgSize)) //need retry load
                break;
            if(ym->Status == ENUM_YMODEM_STA_INFO)          //info pkg
            {
                __Ymodem_InfoPkg(ym);
                Ret = __Ymodem_FileOps(ym, YMODEM_FILE_OPENW);
            }
            else if(ym->Status == CN_YMODEM_EOT)
            {
                // ������һ���ļ��󣬾ͽ�������ΪsecureCRT���ͽ���ʱ�����ݰ��ﲻ��ȫ�㣬
                // �����пո������0����û��ϸ�о����Ⱦ�ֻ֧�ִ���һ���ļ���
                // if(__Ymodem_IsZeroPkg(ym))
                // {
                    write_char(CN_YMODEM_ACK, s_s32gYmodemDevOut);
                    goto YMODEM_RECVEXIT;
                // }
                // else
                // {
                //     __Ymodem_InfoPkg(ym);       //��������һ���ļ�
                //     Ret = __Ymodem_FileOps(ym, YMODEM_FILE_OPENW);
                // }
            }
            else                            //�յ�128�ֽڴ�С�����ݰ�
            {
                if(ym->PkgBuf[1] == (ym->PkgNo & 0xff))
                {
                    ym->PkgNo ++;
                    Ret = __Ymodem_FileOps(ym,YMODEM_FILE_WRITE);
                }
                else
                {
                    write_char(CN_YMODEM_NAK, s_s32gYmodemDevOut);    //���Ŵ������ش�
                }
                ym->Status = CN_YMODEM_SOH;
            }
            break;
        case CN_YMODEM_STX:
            if(!__Ymodem_PackCheck(ym->PkgBuf,ym->PkgSize)) //need retry load
                break;
            if(ym->PkgBuf[1] == (ym->PkgNo & 0xff))
            {
                ym->PkgNo ++;
                Ret = __Ymodem_FileOps(ym,YMODEM_FILE_WRITE);
            }
            else
            {
                write_char(CN_YMODEM_NAK, s_s32gYmodemDevOut);//���Ŵ������ش�
            }
            ym->Status = CN_YMODEM_STX;
            break;
        case CN_YMODEM_EOT:
            if( (ym->Status == CN_YMODEM_SOH) || (ym->Status == CN_YMODEM_STX)) //  ��һ��EOT
            {
                write_char(CN_YMODEM_NAK, s_s32gYmodemDevOut); //���յ����������ظ�ACK
                ym->Status = CN_YMODEM_EOT;
            }
            else if(ym->Status == CN_YMODEM_EOT)
            {
                write_char(CN_YMODEM_ACK, s_s32gYmodemDevOut);                         //���յ����������ظ�ACK
                write_char(CN_YMODEM_C, s_s32gYmodemDevOut);                           //���յ����������ظ�C
            }
            break;
        case CN_YMODEM_CAN:
            Ret = YMODEM_CAN_TRANS;
            goto YMODEM_RECVEXIT;
            break;
        default:
            Ret = YMODEM_UNKNOW_ERR;
            break;
        }
        ym->PkgBufCnt = 0;

        if(Ret != YMODEM_OK)            //������� 1.case 2.file ops
        {
            __Ymodem_CancelTrans();
            break;
        }
    }
YMODEM_RECVEXIT:
    __Ymodem_FileOps(ym,YMODEM_FILE_CLOSE);                 //close file
    return Ret;
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// ============================================================================
// ���ܣ�Ymodem�����ļ����޲�������Ϊ�ļ����Ѿ���ymodemЭ������������
// �������ޣ���ͨ��shell���õ�ǰ·�������ߵ���Ymodem_PathSet����·��
// ���أ�true,���سɹ�������ʧ��
// ============================================================================
//bool_t Ymodem_DownloadFile(char *Param)
bool_t downloadym(char *Param)
{
    YMRESULT Ret = YMODEM_OK;
    u32 CntOver = 0;
    s32 res = 0;

    if(NULL == pYmodem)
    {
        debug_printf("MODULE","YMODEM IS NOT INSTALLED !\r\n");
        return false;
    }
    if(!Lock_MutexPend(pYmodem->pYmodemMutex,CN_TIMEOUT_FOREVER))
        return false;

    pYmodem->File   = NULL;
//  pYmodem->FileOps  = YMODEM_FILE_NOOPS;
    pYmodem->FileSize = 0;
    pYmodem->FileCnt  = 0;
    pYmodem->PkgNo    = 0;
    pYmodem->PkgSize  = CN_YMODEM_SOH_SIZE;
    pYmodem->TimeOut  = CFG_YMODEM_TIMEOUT * 1000 *1000;
    pYmodem->PkgBufCnt = 0;
    pYmodem->FileBufCnt = 0;
    pYmodem->Status   = ENUM_YMODEM_STA_INFO;
    pYmodem->FileBuf  = (u8*)malloc((CFG_YMODEM_BUF_NUM + 1)*1024);
    pYmodem->PkgBuf   = (u8*)malloc(CN_YMODEM_PKGBUF_SIZE);

    if( (NULL == pYmodem->FileBuf) || (NULL == pYmodem->PkgBuf)  )
    {
        Ret = YMODEM_MEM_ERR;
        goto YMODEM_EXIT;
    }

    memset(pYmodem->FileBuf,0,(CFG_YMODEM_BUF_NUM + 1)*1024);
    memset(pYmodem->PkgBuf, 0, CN_YMODEM_PKGBUF_SIZE);
    write_char(CN_YMODEM_C, s_s32gYmodemDevOut);

    //�ȴ������������ݣ���ʱ����
    debug_printf("MODULE","���ص���ʱ��          ");
    fcntl(s_s32gYmodemDevIn, F_SETTIMEOUT, 1000*mS);
    while(( res = read_char( s_s32gYmodemDevIn ) )== EOF)
    {
        if (CntOver++ < 60)
        {
            write_char(CN_YMODEM_C, s_s32gYmodemDevOut); //��ʱ�����·���C
            printf("\b\b\b\b\b%2dS \r\n",60-CntOver);
            continue;
        }
        else
        {
            Ret = YMODEM_TIMEOUT;
            goto YMODEM_EXIT;
        }
    }
    pYmodem->PkgBuf[0] = (u8)res;
    pYmodem->PkgBufCnt = 1;
    pYmodem->StartTime = DJY_GetSysTime();
    fcntl(s_s32gYmodemDevIn, F_SETTIMEOUT, CFG_YMODEM_PKG_TIMEOUT);
    if (strcmp(Param, "CRT") == 0)
    {
        Ret = __Ymodem_ReceiveProcess_CRT(pYmodem);
    }
    else
    {
        Ret = __Ymodem_ReceiveProcess(pYmodem);
    }

YMODEM_EXIT:
    if( NULL != pYmodem->FileBuf)
        free(pYmodem->FileBuf);
    if( NULL != pYmodem->PkgBuf)
        free(pYmodem->PkgBuf);
    fcntl(s_s32gYmodemDevIn, F_SETTIMEOUT, CN_TIMEOUT_FOREVER); //�����stdin���ָ���ʱ����

    Lock_MutexPost(pYmodem->pYmodemMutex);
    if(Ret != YMODEM_OK)                        //��ӡ�����Ϣ
    {
        if(Ret == YMODEM_PARAM_ERR)
        {
            error_printf("MODULE","YMODEM PARAMETER ERR !\r\n");
        }
        else if(Ret == YMODEM_FILE_ERR)
        {
            error_printf("MODULE","YMODEM FILE OPERATION ERR !\r\n");
        }
        else if(Ret == YMODEM_TIMEOUT)
        {
            error_printf("MODULE","YMODEM OPERATION TIMEOUT ERR !\r\n");
        }
        else if(Ret == YMODEM_MEM_ERR)
        {
            error_printf("MODULE","YMODEM NOT ENOUGH MEMORY !\r\n");
        }
        else if(Ret == YMODEM_CAN_TRANS)
        {
            error_printf("MODULE","YMODEM BE CANCELED !\r\n");
        }
        else
        {
            error_printf("MODULE","YMODEM UNKNOW ERR !\r\n");
        }
        return false;
    }
    else
    {
        notice_printf("MODULE","YMODEM SUCCESSED !\r\n");
        return true;
    }
}
#pragma GCC diagnostic pop

static YMRESULT __Ymodem_SendProcess(tagYmodem *ym)
{
    YMRESULT Ret = YMODEM_OK;
    s64 CurrentTime;
    u8 Cmd[8];
    s32 res;
    u32 temp;
    char *FileName;
    char *p1 = NULL;
    char *p2 = NULL;

    while(1)
    {
        if(ym->PkgBufCnt == 1)
        {
            res = ym->PkgBuf[0];
        }
        else if((res = read_char(s_s32gYmodemDevIn)) == EOF)
        {
            Ret = YMODEM_TIMEOUT;
            __Ymodem_CancelTrans();
            break;
        }
        Cmd[0] = (u8)res;
        CurrentTime = DJY_GetSysTime();              //�ܳ�ʱ����
        if(CurrentTime - ym->StartTime >= ym->TimeOut)
        {
            Ret = YMODEM_TIMEOUT;
            __Ymodem_CancelTrans();
            break;
        }
        //��ʼ�������ݰ�
        switch(Cmd[0])
        {
        case CN_YMODEM_C:
            if(ym->Status == ENUM_YMODEM_STA_INFO )         //д�װ�
            {
                p1 = p2 = NULL;
                p1 = strrchr(ym->FileName,'\\'); //case: back slash within path
                p2 = strrchr(ym->FileName,'/'); //case: forward slash within path
                FileName = (p1 < p2) ? p2 : p1;
                if (FileName==NULL) {
                    FileName = ym->FileName;
                }
                else {
                    FileName++; // remove the slash token.
                }
                temp = strlen(FileName);
                ym->PkgSize = CN_YMODEM_SOH_SIZE;
                memset(ym->PkgBuf,0x00,ym->PkgSize + 5);
                ym->PkgBuf[0] = CN_YMODEM_SOH;
                ym->PkgBuf[1] = 0x00;
                ym->PkgBuf[2] = 0xFF;
                memcpy(ym->PkgBuf + 3,FileName,temp);
                ym->PkgBuf[temp + 3] = 0x00;
                itoa(ym->FileSize,(char*)(ym->PkgBuf + temp + 4),10);
                __Ymodem_WriteCrc16(ym->PkgBuf,ym->PkgSize);
                ym->Status = CN_YMODEM_C;       //��ʼ������
                ym->PkgNo = 1;
            }
            else if(ym->PkgNo == 1)     //���͵�һ��
            {
                ym->Status = ENUM_YMODEM_STA_SOH;
                Ret = __Ymodem_FileOps(ym,YMODEM_FILE_READ);
//              ym->FileOps = YMODEM_FILE_READ;
                ym->PkgNo ++;
            }
            else if(ym->Status == CN_YMODEM_EOT)
            {
                ym->PkgSize = CN_YMODEM_SOH_SIZE;
                memset(ym->PkgBuf,0x00,ym->PkgSize + 5);
                ym->PkgBuf[0] = CN_YMODEM_SOH;
                ym->PkgBuf[1] = 0x00;
                ym->PkgBuf[2] = 0xFF;
                __Ymodem_WriteCrc16(ym->PkgBuf,ym->PkgSize);
                ym->Status = ENUM_YMODEM_STA_ZERO;
            }
            break;
        case CN_YMODEM_NAK:                     //�ط����ݰ�
            break;
        case CN_YMODEM_CAN:
            Ret = YMODEM_CAN_TRANS;
            break;
        case CN_YMODEM_ACK:
            if( (ym->PkgNo == 1) )              //�ոշ������װ���Ϣ��
            {
                continue;
            }
            else if(ym->Status == ENUM_YMODEM_STA_ZERO) //�������
            {
                Ret = YMODEM_OK;
                goto YMODEM_SENDEXIT;
            }
            else if(ym->FileCnt >= ym->FileSize)
            {
                write_char(CN_YMODEM_EOT, s_s32gYmodemDevOut);
                ym->Status = CN_YMODEM_EOT;
                continue;
            }
            else
            {

                ym->Status = ENUM_YMODEM_STA_STX;
                Ret = __Ymodem_FileOps(ym,YMODEM_FILE_READ);
                ym->PkgNo ++;
//              ym->FileOps = YMODEM_FILE_READ;
            }
            break;
        default:
            Ret = YMODEM_UNKNOW_ERR;
            break;
        }

        ym->PkgBufCnt = 0;
//      Ret = __Ymodem_FileOps(ym,0);
        if(Ret == YMODEM_OK)
        {
            temp = fwrite(ym->PkgBuf,1,ym->PkgSize + 5,stdout);
            if(temp != ym->PkgSize + 5)
            {
                Ret = YMODEM_MEDIA_ERR;
            }
        }
        if(Ret != YMODEM_OK)        //�������1.case 2.file 3.
        {
            __Ymodem_CancelTrans();
            break;
        }
    }
YMODEM_SENDEXIT:
    return Ret;
}

// ============================================================================
// ���ܣ�Ymodem����API�������ߴ����ļ�����Ϊ��������ȡ�ļ�
// �������ļ���·�����ļ�����
//      1.��Ymodem_PathSet������ʱ��ʹ�þ���·����
//      2.����ʹ�õ������·������ͨ��shell�ȷ�ʽ�ı�·����
// ���أ�true,���سɹ�������ʧ��
// ============================================================================
//bool_t Ymodem_UploadFile(char *Param)
bool_t uploadym(char *Param)
{
    YMRESULT Ret = YMODEM_OK;
    u32 CntOver = 0;
    s32 res = 0;
    char *NextParam,*FileName;

    if(NULL == pYmodem)
    {
        error_printf("MODULE","YMODEM IS NOT INSTALLED !\r\n");
        return false;
    }
    if(!Lock_MutexPend(pYmodem->pYmodemMutex,CN_TIMEOUT_FOREVER))
        return false;
//    TODO:h730û����shell��Ϊflash����������shell_inputsע��
//    FileName = (char *)shell_inputs(Param, &NextParam);
    if(NULL == FileName)
    {
        Ret = YMODEM_PARAM_ERR;
        goto YMODEM_EXIT;
    }
    __Ymodem_FilePathMerge(FileName);
    pYmodem->File   = NULL;
//  pYmodem->FileOps  = YMODEM_FILE_OPEN;
    pYmodem->FileSize = 0;
    pYmodem->FileCnt  = 0;
    pYmodem->PkgNo    = 0;
    pYmodem->PkgSize  = CN_YMODEM_SOH_SIZE;
    pYmodem->TimeOut  = CFG_YMODEM_TIMEOUT * 1000 *1000;;
    pYmodem->PkgBufCnt = 0;
    pYmodem->FileBufCnt = 0;
    pYmodem->Status   = ENUM_YMODEM_STA_INFO;
    pYmodem->FileBuf  = NULL;
    pYmodem->PkgBuf   = (u8*)malloc(CN_YMODEM_PKGBUF_SIZE);

    if( (NULL == pYmodem->PkgBuf)  )
    {
        Ret = YMODEM_MEM_ERR;
        goto YMODEM_EXIT;
    }
    Ret = __Ymodem_FileOps(pYmodem, YMODEM_FILE_OPENR);          //open the file,must be exist
    if(Ret != YMODEM_OK)
    {
        goto YMODEM_EXIT;
    }
//  pYmodem->FileOps  = YMODEM_FILE_STAT;
    Ret = __Ymodem_FileOps(pYmodem,YMODEM_FILE_STAT);          //GET THE FILE SIZE
    if(Ret != YMODEM_OK)
    {
        goto YMODEM_EXIT;
    }

    printf("�ϴ�����ʱ��       ");
    fcntl(s_s32gYmodemDevIn, F_SETTIMEOUT, 1000*mS);
    while((res=read_char(s_s32gYmodemDevIn)) == EOF) //�ȴ������������ݣ���ʱ����
    {
        if (CntOver++ < 60)
        {
            printf("\b\b\b\b%2d s",60-CntOver);
            continue;
        }
        else
        {
            Ret = YMODEM_TIMEOUT;
            goto YMODEM_EXIT;
        }
    }

    pYmodem->PkgBuf[0] = (u8)res;

    pYmodem->PkgBufCnt = 1;
    pYmodem->StartTime = DJY_GetSysTime();
    fcntl(s_s32gYmodemDevIn, F_SETTIMEOUT, CFG_YMODEM_PKG_TIMEOUT);
    Ret = __Ymodem_SendProcess(pYmodem);

//  pYmodem->FileOps = YMODEM_FILE_CLOSE;       //�ر��ļ�
    __Ymodem_FileOps(pYmodem,YMODEM_FILE_CLOSE);

YMODEM_EXIT:
    if( NULL != pYmodem->FileBuf)
        free(pYmodem->FileBuf);
    if( NULL != pYmodem->PkgBuf)
        free(pYmodem->PkgBuf);
    fcntl(s_s32gYmodemDevIn, F_SETTIMEOUT, CN_TIMEOUT_FOREVER);

    Lock_MutexPost(pYmodem->pYmodemMutex);
    if(Ret != YMODEM_OK)                        //��ӡ�����Ϣ
    {
        if(Ret == YMODEM_PARAM_ERR)
        {
            error_printf("MODULE","YMODEM PARAMETER ERR, PLEASE ENTER FILE NAME !\r\n");
        }
        else if(Ret == YMODEM_FILE_ERR)
        {
            error_printf("MODULE","YMODEM FILE OPERATION ERR !\r\n");
        }
        else if(Ret == YMODEM_TIMEOUT)
        {
            error_printf("MODULE","YMODEM OPERATION TIMEOUT ERR !\r\n");
        }
        else if(Ret == YMODEM_MEM_ERR)
        {
            error_printf("MODULE","YMODEM NOT ENOUGH MEMORY !\r\n");
        }
        else if(Ret == YMODEM_CAN_TRANS)
        {
            error_printf("MODULE","YMODEM BE CANCELED !\r\n");
        }
        else
        {
            error_printf("MODULE","YMODEM UNKNOW ERR !\r\n");
        }
        return false;
    }
    else
    {
        error_printf("MODULE","YMODEM SUCCESSED !\r\n");
        return true;
    }

    return true;
}

bool_t downloadym_app(char *Param)
{
    Ymodem_PathSet("/xip-app/");
    return downloadym(Param);
}

ADD_TO_ROUTINE_SHELL(upload ,uploadym,"�ϴ��ļ�    �����ʽ: upload �ļ���\n\r");
ADD_TO_ROUTINE_SHELL(downloada ,downloadym_app,"����app�ļ�       �����ʽ: downloada");


