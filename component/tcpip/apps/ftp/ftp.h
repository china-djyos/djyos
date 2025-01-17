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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

#ifndef __FTP_H
#define __FTP_H

#define CN_FILEREBOOT                 110   //新文件指示器上的重启标记
#define CN_SERVERREADYTIME            120   //服务器准备就绪的时间（分钟数）
#define CN_OPENDATACONNECTION125      125   //打开数据连接，开始传输
#define CN_OPENCONNECTION             150   //打开连接
#define CN_CMDSUCCESS                 200   //成功
#define CN_CMDFAILED                  202   //命令没有执行
#define CN_SYSSTAT                    211   //系统状态回复
#define CN_DIRSTAT                    212   //目录状态回复
#define CN_FILESTAT                   213   //文件状态回复
#define CN_HELPSTAT                   214   //帮助信息回复
#define CN_SYSTYPE                    215   //系统类型回复
#define CN_SERVERREADY                220   //服务就绪
#define CN_LOGOUT                     221   //退出网络
#define CN_OPENDATACONNECTION225      225   //打开数据连接
#define CN_CLOSEDATACONNECTION        226   //结束数据连接
#define CN_ENTERPASSIVE               227   //进入被动模式（IP 地址、ID 端口）
#define CN_LOGIN                      230   //登录因特网
#define CN_FILECOMPLETE               250   //文件行为完成
#define CN_PATHBUILD                  257   //路径名建立
#define CN_NEEDPASS                   331   //要求密码
#define CN_NEEDUSER                   332   //要求帐号
#define CN_FILEPAUSE                  350   //文件行为暂停
#define CN_SERVERCLOSE                421   //服务关闭
#define CN_OPENDATACONNECTIONFAILED   425   //无法打开数据连接
#define CN_CLOSECONNECTION            426   //结束连接
#define CN_FILENOTVALID               450   //文件不可用
#define CN_LOCALERR                   451   //遇到本地错误
#define CN_SPACENOTENOUGH             452   //磁盘空间不足
#define CN_CMDINVALID                 500   //无效命令
#define CN_PARAERR                    501   //错误参数
#define CN_CMDNOTEXE                  502   //命令没有执行
#define CN_CMDORDERERR                503   //错误指令序列
#define CN_CMDPARAINVALID             504   //无效命令参数
#define CN_NOLOG                      530   //未登录网络
#define CN_STRNEEDUSER                532   //存储文件需要帐号
#define CN_FILEINVALID                550   //文件不可用
#define CN_UNKOWNPAGE                 551   //不知道的页类型
#define CN_EXCEEDSTORAGED             552   //超过存储分配
#define CN_FILENAMEDENI               553   //文件名不允许
#define CN_RESPONSCODEEERR            0     //no response code get
#define CN_SNDCMDERR                  -1    //send the command err


#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>

typedef enum
{
    EN_FTP_STAT_OK = 0,  //NO ERR here
    EN_FTP_STAT_RCVERR,  //RECV ERR
    EN_FTP_STAT_SNDERR,  //SND ERR
    EN_FTP_STAT_CONERR,  //CONNECT ERR
    EN_FTP_STAT_OPENERR, //OPEN FILE ERR
    EN_FTP_STAT_READERR, //READ ERR
    EN_FTP_STAT_WRITEERR,//WRITE ERR
    EN_FTP_STAT_CMDERR,  //COMMAND ERR
}enFtpErrCode;

//define for the client data structure
#define CN_FTPCLIENT_BUFLEN   512
#define CN_FTPCLIENT_TIMEOUT  (60*1000*mS)  //60 seconds
#define CN_FTPCLIENT_TRYTIMES (3)          //ten times
#define CN_FTP_NAMELEN        (32)
#define CN_PATH_LENGTH        128          //USE TO LIMITE THE DIR PATH LEN

//define the structure for the control channel
typedef struct
{
    s32             s;      //control socket
    struct in_addr  ipaddr; //ftpd：client IP addr;  ftpc：???
    u16             port;   //ftpd：client port;     ftpc：???
}tagChannelCtrl;            //control channel
//define the structure for the data channel
//几个socket的连接过程：
//1、首先创建socket server21port，bind 21号端口，listen之。
//2、客户端连接21端口，Accept得到 cchannel.s
//3、cchannel.s进行ftp会话，例如登录。
//4、接到pasv或port命令，分配端口创建 dchannel.sli
typedef struct
{
    bool_t ispasv;          //true = pasv mode，false =  port mode
    struct in_addr  ipaddr; //net bytes
    u16             port;   //net bytes
    s32             slisten; //执行“PASV”时建立的数据连接
    s32             saccept; //port socket(return by accept)
    s32             sconnect;//pasv socket(return by connect)
    char            type;    //type mode
}tagChannelData;//data channel
//define the structure for the ftp client controller
typedef struct __FtpClient
{
    struct __FtpClient *nxt;
    tagChannelCtrl cchannel;                //控制通道
    tagChannelData dchannel;                //数据通道
    char         ipadr[CN_FTP_NAMELEN];      //server address
    char         user[CN_FTP_NAMELEN];       //user
    char         passwd[CN_FTP_NAMELEN];     //passwd
    char         port[CN_FTP_NAMELEN];       //port
    char         curdir[CN_PATH_LENGTH];     //本地当前各种目录
    u8           buf[CN_FTPCLIENT_BUFLEN]  ; //used for the send and recv buffer
    s32          buflen;     //which means how long the buffer is
    s32          datalen;    //which means how many data in the buffer
    s32          errcode;    //which means the last state
}tagFtpDomain;
void FTP_ShowClient(tagFtpDomain *client);
s32 FTP_RcvLine(s32 sock,u8 *buf,s32 len);
s32 FTP_Connect(struct in_addr *addr,u16 port);
s32 FTP_Accept(s32 s,struct in_addr *ipaddr,u16 *port);


#endif /* __FTP_H */
