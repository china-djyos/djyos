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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块:ymodem模块
//作者:  贺敏.
//版本：V2.0.0
//文件描述:ymodem模块，利用ymodem协议上传或下载文件
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2013-10-06
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
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
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_Ymodem(void);
//    ModuleInstall_Ymodem();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"ymodem"       //ymodem
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system","file system"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_YMODEM == false )
//#warning  " ymodem  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_YMODEM    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
#define     CFG_YMODEM_BUF_NUM      32              //"缓存ymodem包数量",每包1024字节
//%$#@num,1,100000000,
#define     CFG_YMODEM_PKG_TIMEOUT  (15*1000*1000)  //"包间隔超时时间",微秒
#define     CFG_YMODEM_TIMEOUT      (300) //"ymodem传输总超时时间",秒
//%$#@enum,true,false,
//%$#@string,1,128,
#define     CFG_YMODEM_DEVNAME      "std"           //"ymodem传输设备，std表示使用标准输入输出设备"
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
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

// YMODEM命令
#define CN_YMODEM_SOH     1         //128字节起始符
#define CN_YMODEM_STX     2         //1028字节起始符
#define CN_YMODEM_EOT     4
#define CN_YMODEM_ACK     6
#define CN_YMODEM_NAK     0x15
#define CN_YMODEM_C       0x43          //大写字母C
#define CN_YMODEM_CAN     0x18

// YMODEM数据包长度
#define CN_YMODEM_SOH_SIZE  128        //SOH数据包大小为128字节
#define CN_YMODEM_STX_SIZE  1024       //STX数据包大小为1024

typedef enum
{
     YMODEM_OK= 0,              //正常返回
     YMODEM_PARAM_ERR,          //参数错误
     YMODEM_FILE_ERR,           //文件操作错误
     YMODEM_TIMEOUT,            //超时返回
     YMODEM_MEM_ERR,            //内存不够
     YMODEM_CAN_TRANS,          //取消传输
     YMODEM_MEDIA_ERR,          //硬件错误
     YMODEM_UNKNOW_ERR
}YMRESULT;

typedef enum
{
    ENUM_YMODEM_STA_INFO = 0x80,        //YMODEM信息包
    ENUM_YMODEM_STA_SOH,                //128字节的数据包
    ENUM_YMODEM_STA_STX,                //1024字节的数据包
    ENUM_YMODEM_STA_EOT,                //结束符
    ENUM_YMODEM_STA_ZERO,               //全ZERO包
}YMSTATUS;

typedef enum
{
    YMODEM_FILE_NOOPS= 0xF0,            //无文件操作
    YMODEM_FILE_OPENW ,                 //打开文件用于写
    YMODEM_FILE_OPENR ,                 //打开文件用于读
    YMODEM_FILE_WRITE,                  //写文件标记
    YMODEM_FILE_READ,                   //读文件标记
    YMODEM_FILE_STAT,
    YMODEM_FILE_CLOSE,                  //关闭文件标记
}YMFILEOPS;


typedef struct __Ymodem
{
    FILE    *File;                      //file ptr for file op
//  const char    *Path;                //文件所在的路径（未含文件名）
    char    *FileName;                  //文件名称
    struct MutexLCB *pYmodemMutex;      //互斥访问
//  YMFILEOPS FileOps;                  //文件操作
    u32     FileSize;                   //当前传输文件大小
    u32     FileCnt;                    //写入文件大小计数
    u32     PkgNo;                      //包号
    u32     PkgSize;                    //当前包长度
    s64     StartTime;                  //start time
    u32     TimeOut;                    //time out
    YMSTATUS Status;                    //ymodem status
    u8      *FileBuf;                   //file buf
    u32     FileBufCnt;                 //文件缓冲区大小计数
    u8      *PkgBuf;
    u32     PkgBufCnt;                  //PKG大小计数
}tagYmodem;

bool_t uploadym(char *Param);
bool_t downloadym(char *Param);

static tagYmodem *pYmodem = NULL;
static s32 s_s32gYmodemDevIn,s_s32gYmodemDevOut;

//----ymodem模型初始化---------------------------------------
//功能: ymodem下载文件模块接口函数，在module_trim中调用
//参数: 无
//返回: 出错则返回false
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

//----获取下载数据-------------------------------------------
//功能：从数据通道接收一包数据
//参数：
//返回：
//备注：串口驱动阻塞机制变更，这里逻辑不变，即没有读到len长度时，
//     会一直等待timeout
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
// 功能：配置ymodem下载文件的绝对路径，该函数选择性调用，
//      1.若应用需要变更下载路径，则不调用该函数，通过其他方式（如shell）修改路径;
//      2.调用该函数后，通过Ymodem下载的路径则不可变换;
//      3.举例说明：如利用iboot升级，下载路径不需变，因此Path = "/iboot"
// 参数：Path，通过Ymodem存放的绝对路径
// 返回：true,成功，否则，失败
// ============================================================================
bool_t Ymodem_PathSet(const char *Path)
{
    bool_t Ret = false;
    u32 len;

    if( NULL != Path )
    {
        len = strlen(Path);
        // -2是因为Path最后一个字符不是'/'，需要补足
        if(len < CN_YMODEM_NAME_LENGTH - 2)
        {
            if(Lock_MutexPend(pYmodem->pYmodemMutex,CN_TIMEOUT_FOREVER))
            {
        //      pYmodem->Path = Path;
                strcpy(pYmodem->FileName, Path);
                //最后一个字符不是'/'或'\\'，需要补上
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

//----提取单词----------------------------------------------
//功能: 从buf中提取一个由空格或行结束符隔开的单词，next用于
//      返回下一个单词首地址，如果没有下一个单词，则next=NULL。
//参数: buf，待分析的字符串
//      next，返回下一个单词指针
//返回: 提取的单词指针，已将单词后面的分隔符换成串结束符'\0'
//-----------------------------------------------------------
//todo:shell.c中Sh_GetWord函数与此函数雷同。
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
//------ymodem取消传输---------------------------------------
//功能：ymodem模块通知对端取消本次传输
//参数：无
//返回：无
//-----------------------------------------------------------
static void __Ymodem_CancelTrans(void)
{
    write_char(CN_YMODEM_CAN, s_s32gYmodemDevOut);
    write_char(CN_YMODEM_CAN, s_s32gYmodemDevOut);
    write_char(CN_YMODEM_CAN, s_s32gYmodemDevOut);
}

//------校验ymodem数据包-------------------------------------
//功能：ymodem模块校验接收到的数据包
//参数：buf：数据包缓冲
//      pack_len: ymodem数据包长度
//返回：true:  数据包校验正确
//      false: 数据包校验错误
//----------------------------------------------------------
static bool_t __Ymodem_PackCheck(u8* buf, u32 pack_len)
{
    u16 checksum,check;
    if((buf[1] + buf[2]) != 0xff)               //校验包号正反码
    {
        write_char(CN_YMODEM_NAK, s_s32gYmodemDevOut);     //应答nak，请求重发
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
    if(checksum != check)    //CRC校验错误
    {
        write_char(CN_YMODEM_NAK, s_s32gYmodemDevOut);                    //应答nak，请求重发
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

// 获取完整的YMODEM数据包，包括数据包和ACK、CAN等包
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

    //对文件进行操作
    switch(cmd)
    {
    case YMODEM_FILE_OPENW:
        ym->File = fopen(ym->FileName,"w+");        //打开文件，不存在则创建
        write_char(CN_YMODEM_ACK, s_s32gYmodemDevOut);
        write_char(CN_YMODEM_C, s_s32gYmodemDevOut);
        if(ym->File == NULL)
        {
            Ret = YMODEM_FILE_ERR;
        }
        break;
    case YMODEM_FILE_OPENR:
        ym->File = fopen(ym->FileName,"r");         //打开文件
        if(ym->File == NULL)
        {
            Ret = YMODEM_FILE_ERR;
        }
        break;
    case YMODEM_FILE_WRITE:
        memcpy(ym->FileBuf + ym->FileBufCnt,ym->PkgBuf + 3,ym->PkgSize);
        ym->FileBufCnt += ym->PkgSize;
        if((ym->FileBufCnt >= ym->FileSize - ym->FileCnt) ||
                (ym->FileBufCnt >= CFG_YMODEM_BUF_NUM*1024)) //如果足够大，写入flash
        {
            if(ym->FileBufCnt >= ym->FileSize - ym->FileCnt)                                   //判断本次写入大小
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
        CurrentTime = DJY_GetSysTime();                  //总超时处理
        if(CurrentTime - ym->StartTime >= ym->TimeOut)
        {
            Ret = YMODEM_TIMEOUT;
            __Ymodem_CancelTrans();
            break;
        }
        //数据包处理
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
                    write_char(CN_YMODEM_ACK, s_s32gYmodemDevOut);//全零包，所有传输结束
                    goto YMODEM_RECVEXIT;
                }
                else
                {
                    __Ymodem_InfoPkg(ym);       //继续传下一个文件
                    Ret = __Ymodem_FileOps(ym, YMODEM_FILE_OPENW);
                }
            }
            else                            //收到128字节大小的数据包
            {
                if(ym->PkgBuf[1] == (ym->PkgNo & 0xff))
                {
                    ym->PkgNo ++;
                    Ret = __Ymodem_FileOps(ym,YMODEM_FILE_WRITE);
                }
                else
                {
                    write_char(CN_YMODEM_NAK, s_s32gYmodemDevOut);    //包号错误，需重传
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
                write_char(CN_YMODEM_NAK, s_s32gYmodemDevOut);//包号错误，需重传
            }
            ym->Status = CN_YMODEM_STX;
            break;
        case CN_YMODEM_EOT:
            if( (ym->Status == CN_YMODEM_SOH) || (ym->Status == CN_YMODEM_STX)) //  第一个EOT
            {
                write_char(CN_YMODEM_NAK, s_s32gYmodemDevOut); //接收到结束符，回复ACK
                ym->Status = CN_YMODEM_EOT;
            }
            else if(ym->Status == CN_YMODEM_EOT)
            {
                write_char(CN_YMODEM_ACK, s_s32gYmodemDevOut);                         //接收到结束符，回复ACK
                write_char(CN_YMODEM_C, s_s32gYmodemDevOut);                           //接收到结束符，回复C
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

        if(Ret != YMODEM_OK)            //可能情况 1.case 2.file ops
        {
            __Ymodem_CancelTrans();
            break;
        }
    }
YMODEM_RECVEXIT:
    __Ymodem_FileOps(ym,YMODEM_FILE_CLOSE);                 //close file
    return Ret;
}

// 用secureCRT使用ymodem时，用这个函数接收数据。
// 暂时只能接收一个文件
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
        CurrentTime = DJY_GetSysTime();                  //总超时处理
        if(CurrentTime - ym->StartTime >= ym->TimeOut)
        {
            Ret = YMODEM_TIMEOUT;
            __Ymodem_CancelTrans();
            break;
        }
        //数据包处理
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
                // 传输完一个文件后，就结束，因为secureCRT发送结束时，数据包里不是全零，
                // 而是有空格和数字0，赞没仔细研究，先就只支持传输一个文件。
                // if(__Ymodem_IsZeroPkg(ym))
                // {
                    write_char(CN_YMODEM_ACK, s_s32gYmodemDevOut);
                    goto YMODEM_RECVEXIT;
                // }
                // else
                // {
                //     __Ymodem_InfoPkg(ym);       //继续传下一个文件
                //     Ret = __Ymodem_FileOps(ym, YMODEM_FILE_OPENW);
                // }
            }
            else                            //收到128字节大小的数据包
            {
                if(ym->PkgBuf[1] == (ym->PkgNo & 0xff))
                {
                    ym->PkgNo ++;
                    Ret = __Ymodem_FileOps(ym,YMODEM_FILE_WRITE);
                }
                else
                {
                    write_char(CN_YMODEM_NAK, s_s32gYmodemDevOut);    //包号错误，需重传
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
                write_char(CN_YMODEM_NAK, s_s32gYmodemDevOut);//包号错误，需重传
            }
            ym->Status = CN_YMODEM_STX;
            break;
        case CN_YMODEM_EOT:
            if( (ym->Status == CN_YMODEM_SOH) || (ym->Status == CN_YMODEM_STX)) //  第一个EOT
            {
                write_char(CN_YMODEM_NAK, s_s32gYmodemDevOut); //接收到结束符，回复ACK
                ym->Status = CN_YMODEM_EOT;
            }
            else if(ym->Status == CN_YMODEM_EOT)
            {
                write_char(CN_YMODEM_ACK, s_s32gYmodemDevOut);                         //接收到结束符，回复ACK
                write_char(CN_YMODEM_C, s_s32gYmodemDevOut);                           //接收到结束符，回复C
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

        if(Ret != YMODEM_OK)            //可能情况 1.case 2.file ops
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
// 功能：Ymodem下载文件，无参数，因为文件名已经在ymodem协议数据名里面
// 参数：无，需通过shell配置当前路径，或者调用Ymodem_PathSet配置路径
// 返回：true,下载成功，否则，失败
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

    //等待主机发送数据，超时返回
    debug_printf("MODULE","下载倒计时：          ");
    fcntl(s_s32gYmodemDevIn, F_SETTIMEOUT, 1000*mS);
    while(( res = read_char( s_s32gYmodemDevIn ) )== EOF)
    {
        if (CntOver++ < 60)
        {
            write_char(CN_YMODEM_C, s_s32gYmodemDevOut); //超时则重新发送C
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
    fcntl(s_s32gYmodemDevIn, F_SETTIMEOUT, CN_TIMEOUT_FOREVER); //如果是stdin，恢复超时参数

    Lock_MutexPost(pYmodem->pYmodemMutex);
    if(Ret != YMODEM_OK)                        //打印输出信息
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
        CurrentTime = DJY_GetSysTime();              //总超时处理
        if(CurrentTime - ym->StartTime >= ym->TimeOut)
        {
            Ret = YMODEM_TIMEOUT;
            __Ymodem_CancelTrans();
            break;
        }
        //开始发送数据包
        switch(Cmd[0])
        {
        case CN_YMODEM_C:
            if(ym->Status == ENUM_YMODEM_STA_INFO )         //写首包
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
                ym->Status = CN_YMODEM_C;       //开始发数据
                ym->PkgNo = 1;
            }
            else if(ym->PkgNo == 1)     //发送第一包
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
        case CN_YMODEM_NAK:                     //重发数据包
            break;
        case CN_YMODEM_CAN:
            Ret = YMODEM_CAN_TRANS;
            break;
        case CN_YMODEM_ACK:
            if( (ym->PkgNo == 1) )              //刚刚发送完首包信息包
            {
                continue;
            }
            else if(ym->Status == ENUM_YMODEM_STA_ZERO) //传输结束
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
        if(Ret != YMODEM_OK)        //可能情况1.case 2.file 3.
        {
            __Ymodem_CancelTrans();
            break;
        }
    }
YMODEM_SENDEXIT:
    return Ret;
}

// ============================================================================
// 功能：Ymodem上载API，调用者传入文件名作为参数，读取文件
// 参数：文件名路径或文件名，
//      1.当Ymodem_PathSet被调用时，使用绝对路径；
//      2.否则使用的是相对路径，可通过shell等方式改变路径；
// 返回：true,上载成功，否则，失败
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
//    TODO:h730没有用shell因为flash不够。所以shell_inputs注释
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

    printf("上传倒计时：       ");
    fcntl(s_s32gYmodemDevIn, F_SETTIMEOUT, 1000*mS);
    while((res=read_char(s_s32gYmodemDevIn)) == EOF) //等待主机发送数据，超时返回
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

//  pYmodem->FileOps = YMODEM_FILE_CLOSE;       //关闭文件
    __Ymodem_FileOps(pYmodem,YMODEM_FILE_CLOSE);

YMODEM_EXIT:
    if( NULL != pYmodem->FileBuf)
        free(pYmodem->FileBuf);
    if( NULL != pYmodem->PkgBuf)
        free(pYmodem->PkgBuf);
    fcntl(s_s32gYmodemDevIn, F_SETTIMEOUT, CN_TIMEOUT_FOREVER);

    Lock_MutexPost(pYmodem->pYmodemMutex);
    if(Ret != YMODEM_OK)                        //打印输出信息
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

ADD_TO_ROUTINE_SHELL(upload ,uploadym,"上传文件    命令格式: upload 文件名\n\r");
ADD_TO_ROUTINE_SHELL(downloada ,downloadym_app,"下载app文件       命令格式: downloada");


