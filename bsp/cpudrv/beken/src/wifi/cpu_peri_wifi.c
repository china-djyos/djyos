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
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <error.h>
#include <cpu_peri.h>
#include <board-config.h>
#include <sys/socket.h>
#include <netbsp.h>
#include <dbug.h>
#include "shell.h"
#include "app.h"
#include "rwnx.h"
#include "role_launch.h"
#include "djytimer.h"
#include "djy_wifi_config.h"
#include "ieee802_11_demo.h"
#include "wlan_ui_pub.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中

//%$#@end initcode  ****初始化代码结束
//%$#@describe      ****组件描述开始
//component name:"cpu onchip MAC"//CPU的mac驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"tcpip","lock","int","tcpip"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_MAC == false )
//#warning  " cpu_onchip_MAC  组件参数未配置，使用默认配置"
//%$#@target = header    //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_MAC    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,1000,10000,
#define CFG_ETH_LOOP_CYCLE      1000         //"网卡轮询接收周期",
//%$#@enum,true,false,
#define CFG_ETH_LOOP_ENABLE     true        //"网卡接收是否轮询",
//%$#@string,1,32,
#define CFG_ETH_DEV_NAME        "BK7251_WiFi"//"网卡名称",
//%$#@string,1,100,
#define CFG_FAST_DATA_FILE_NAME "/efs/fast_data.dat"    //存快联信息的文件路径
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure
// =============================================================================

#define CN_DEVNAME_LEN 32
//this is the mac receive hook, if any hook rcv,then the data will goto the hook
//the receive data goto the stack default
typedef bool_t (*fnRcvDeal)(u8 *buf, u16 len);

typedef struct
{
    vu32  Ints;                     //the int counter
    vu32  rcvInts;                  //rcv ints counter
    vu32  dmaerr;                   //dmaerr ins counter
    vu32  rxoverInts;               //rcv over ints counter
    vu32  rcvPkgTimes;              //rcv package times
    vu32  sndTimes;                 //the snd times counter
    vu32  sndOkTimes;               //the send success counter
    vu32  sndnobdCnt;               //not enough bd rings
    vu32  sndPkgTooLongCnt;         //snd pkg too long ounter
    vu32  sndInts;                  //the send ints counter
    vu32  rxTaskRun;                //the rx task run counter
    vu32  rsttimes;                 //the reset counter
    vu32  nopkgCnts;                //the no pkg free
}tagMacDebug;

typedef struct
{
    //os member
    struct SemaphoreLCB     *sendsync;          //activate the receive task
    struct MutexLCB         *protect;          //protect the device
    struct NetDev           *devhandle;        //returned by the tcpip stack
    char                    devname[CN_DEVNAME_LEN];
    //hardware
    //which used to descript the device or control the device
    struct devstat
    {
        vu32 pm:1;        //1 copy all the frame ,no addr filter
        vu32 bfd:1;       //1 broadcast frames disable
        vu32 duplex:1;    //1 full while 0 half
        vu32 speed:1;     //1 means 100Mb while 0 10 Mb
        vu32 mii:1;       //1 means mii mode while 0 rmii mode
    }macstat;
    u8            macaddr[CN_MACADDR_LEN];    //storage the mac address
    //dev method
    vu32 loop:1;          //1 use the loop mode while 0 interrupt mode
    vu32 loopcycle:31;    //unit:ms
    fnRcvDeal     fnrcvhook;                  //rcv hook function
    tagMacDebug   debuginfo;                  //debug info
}tagMacDriver;
static tagMacDriver  gMacDriver;

struct NetPbuf {
  /** next pbuf in singly linked pbuf chain */
  struct NetPbuf *next;

  void *payload;

  uint16_t tot_len;

  /** length of this buffer */
  uint16_t len;

  /** pbuf_type as u8_t instead of enum to save space */
  uint8_t /*pbuf_type*/ type;

  uint8_t flags;

  uint16_t ref;
};

struct BusMessage
{
    uint32_t type;
    uint32_t arg;
    uint32_t len;
    struct SemaphoreLCB* sema;
};

#define EthTxBufSize    1524
static u8 gTxBuffer[EthTxBufSize];      //for sending copy frame
static struct NetPbuf  tx_pbuf;
static int gQuickConnect = 0;
extern WIFI_CORE_T g_wifi_core;
void SetQuickConnectFlag(int val)
{
    gQuickConnect = val;
}
int GetQuickConnectFlag()
{
    return gQuickConnect;
}
//mac control function
static bool_t MacCtrl(struct NetDev *devhandle,u8 cmd,ptu32_t para)
{
    bool_t result = false;
    tagMacDriver   *pDrive;
#if 1
    pDrive = &gMacDriver;
    if((pDrive->devhandle == devhandle)&&\
        (Lock_MutexPend(pDrive->protect,CN_TIMEOUT_FOREVER)))
    {
        switch(cmd)
        {
            case EN_NETDEV_SETNOPKG:
                result = true;
                break;
            case EN_NETDEV_SETBORAD:
                result = true;
                break;
            case EN_NETDEV_SETPOINT:
                result = true;
                break;
            case EN_NETDEV_SETMULTI:
               result = true;
                break;
            case EN_NETDEV_SETRECV:
                result = true;
                break;
            case EN_NETDEV_SETSEND:
                break;
            case EN_NETDEV_SETMAC:
                memcpy(pDrive->macaddr,(u8 *)para, CN_MACADDR_LEN);
                pDrive->debuginfo.rsttimes++;
                result = true;
                break;
            case EN_NETDEV_SETMULTIMAC:
                break;
            case EN_NETDEV_GTETMAC:
                if((u8 *)para != NULL)
                {
                    memcpy((u8 *)para,pDrive->macaddr, CN_MACADDR_LEN);
                    result = true;
                }
                break;
            case EN_NETDEV_RESET:
                break;
            case (EN_NETDEV_ADDRFILTER):    //增加一条是否接收所有包
                break;
            default:
                result = false;
                break;
        }
        Lock_MutexPost(pDrive->protect);
    }

    return result;
#endif
    return true;
}
#if 0
int hexdump(int tag, const unsigned char *buf, int size)
{
    int i;
    if(tag)
        printf("===>>>\r\n");
    else
        printf("<<<===\r\n");
    for(i=0; i<size; i++)
        printf("%02x,%s", buf[i], (i+1)%16?"":"\r\n");
    return 0;
}
#endif
//extern void hexdump(const unsigned char* buf, int len);
static bool_t __MacSnd(void* handle,struct NetPkg * pkg,u32 netdevtask)
{
    bool_t             result;
    tagMacDriver      *pDrive;
    struct NetPkg         *tmppkg;
    struct BusMessage     msg;
    struct pbuf         *p;
    u8                *dst,*src;
    u16                len=0;
    int ret = 0;

    result = false;
    pDrive = &gMacDriver;
    pDrive->debuginfo.sndTimes++;
    if((NULL == handle)||(NULL == pkg))
        return result;

    if(Lock_MutexPend(pDrive->protect,CN_TIMEOUT_FOREVER))
    {
        //copy datas to static frame buffer
        tmppkg = pkg;
        dst      = &gTxBuffer[0];
        len = PkgFrameDataCopy(tmppkg, dst);

        if(len <= EthTxBufSize)
        {
            p = pbuf_alloc(PBUF_RAW, len, PBUF_RAM);
            if(p) {
                src = &gTxBuffer[0];
                memcpy(p->payload,src,len);
                //hexdump(1, src,len);
                msg.type = BMSG_TX_TYPE;
                msg.arg = (uint32_t)p;
                msg.len = 0;
    //            msg.sema = gMacDriver.sendsync;
                msg.sema = NULL;
    //            Lock_SempPend(gMacDriver.sendsync,10*mS);
                ret = bk_rtos_push_to_queue(&g_wifi_core.io_queue, &msg, 1 * SECONDS);
                if(0 != ret)
                {
                    result = false;
                }
                else
                {
                    result = true;
                }
            }
            else {
                result = false;
            }
        }
        else
        {
            pDrive->debuginfo.sndPkgTooLongCnt++;
        }


        Lock_MutexPost(pDrive->protect);
    }

    return result;
}

void MacRcv(void *p)
{
    struct NetPbuf *pbuf = (struct NetPbuf *)p;
    struct NetPkg  *pkg = NULL;
    tagMacDriver      *pDrive;
    u8   *dst,*src;
    u8        *rawbuf;
    u16        len;

    pDrive = &gMacDriver;

    if(pbuf->len>0)
    {
        pkg = PkgMalloc(pbuf->len,CN_PKLGLST_END);
    }
    if(NULL != pkg)
    {
        dst = PkgGetCurrentBuffer(pkg);
        src = pbuf->payload;
        memcpy(dst,src,pbuf->len);
        PkgSetDataLen(pkg, pbuf->len);
        if(NULL != pDrive->fnrcvhook)
        {
            rawbuf = PkgGetCurrentBuffer(pkg);
//                  rawbuf = pkg->buf + pkg->offset;
            len = PkgGetDataLen(pkg);
            pDrive->fnrcvhook(rawbuf,len);
        }
        else
        {
            Link_NetDevPush(pDrive->devhandle,pkg);
        }
        PkgTryFreePart(pkg);
    }
}

void DjyWifi_StaConnect(char *ssid,char *connect_key)
{
    demo_sta_app_init(ssid, connect_key);
}

void DjyWifi_StaDisConnect(void)
{
    bk_wlan_stop(STATION);
    DJY_EventDelay(300*mS);//加个延时，如果断开立即快连，有时发不出去数据。
}

void DjyWifi_ApOpen(char *ap_ssid, char *ap_key)
{
    demo_softap_app_init(ap_ssid,ap_key);
}

void DjyWifi_ApClose(void)
{
    bk_wlan_stop(SOFT_AP);
}

extern int wpa_get_psk(char *psk);
extern int wpa_get_passphrase_md5(unsigned char *md5_passphrase, int len);
void DjyWifi_StaConnectDone(void)
{
    LinkStatusTypeDef link_status;
    struct wlan_fast_connect ap_info;
    uint8_t len = 0;
    if ((bk_wlan_get_link_status(&link_status) == kNoErr) && (SECURITY_TYPE_WEP != link_status.security))
    {
        printf("info: %s, Write Quick Info Now!!!\r\n", __FUNCTION__);
        memset(&ap_info, 0, sizeof(struct wlan_fast_connect));//必须清空
        len = strnlen(link_status.ssid, 32);
        if(len<32) len += 1;
        memcpy(ap_info.ssid, link_status.ssid, len);
        memcpy(ap_info.bssid, link_status.bssid, 6);
        ap_info.channel = link_status.channel;
        ap_info.security = link_status.security;
        wpa_get_psk(ap_info.psk);
        wpa_get_passphrase_md5(ap_info.md5_passphrase, sizeof(ap_info.md5_passphrase));
        wlan_fast_connect_info_write(&ap_info);
    }
}

int GetConnectedRssiValue()
{
    LinkStatusTypeDef link_status;
    int ret = 0;
    if (bk_wlan_get_link_status(&link_status) == kNoErr) {
            if ((link_status.conn_state & MSG_GOT_IP) || (link_status.conn_state & MSG_CONN_SUCCESS)) {
                //printf("%s:ch[%d], rssi[%d]!\r\n", link_status.ssid, link_status.channel, link_status.wifi_strength);
                ret = link_status.wifi_strength;
            }
    }
    return ret;
}

int wlan_fast_info_match(char *ssid, char *passwd, wlan_fast_connect_t *out_info);
int wpa_set_passphrase_md5(char *passphrase);
void DjyWifi_StaAdvancedConnect(char *ssid,char *connect_key)
{
    int i = 0;
    char tmp[68] = {0};
    network_InitTypeDef_adv_st  wNetConfigAdv;
    struct wlan_fast_connect ap_info;
    memset(&wNetConfigAdv, 0, sizeof(network_InitTypeDef_adv_st));
    memset(&ap_info, 0, sizeof(struct wlan_fast_connect));
    memset(tmp, 0, sizeof(tmp));
    wpa_set_passphrase_md5(connect_key);
    if (wlan_fast_info_match(ssid, connect_key, &ap_info)) {
        SetQuickConnectFlag(1);
        printf("info: %s, Do Quick WiFi Connect, ssid=%s, connect_key=%s\r\n", __FUNCTION__, ssid, connect_key);
        strncpy(wNetConfigAdv.ap_info.ssid, ap_info.ssid, 32);
        memcpy(wNetConfigAdv.ap_info.bssid, ap_info.bssid, 6);
        wNetConfigAdv.ap_info.security = ap_info.security;
        wNetConfigAdv.ap_info.channel = ap_info.channel;
        for (i = 0; i < 32; i ++) {
            sprintf(&tmp[i * 2], "%02x",  ap_info.psk[i]);
        }
        memcpy(wNetConfigAdv.key, tmp, 64);
        wNetConfigAdv.key_len =  64; // strnlen(passwd, 32);
        wNetConfigAdv.dhcp_mode = DHCP_CLIENT;
        wNetConfigAdv.wifi_retry_interval = 100;
        bk_wlan_start_sta_adv(&wNetConfigAdv);
    }
    else {
        SetQuickConnectFlag(0);
        printf("info: %s, Do Normal WiFi Connect, ssid=%s, connect_key=%s\r\n", __FUNCTION__, ssid, connect_key);
        demo_sta_app_init(ssid, connect_key);
    }

}

void DjyWifi_StartScan(FUNC_2PARAM_PTR ind_cb)
{
    bk_wlan_scan_ap_reg_cb(ind_cb);
    bk_wlan_start_scan();
}

uint32_t DjyWifi_GetScanResult(struct sta_scan_res **scan_result)
{
    struct sta_scan_res *scan_rst_table;
    char scan_rst_ap_num = 0;
    scan_rst_ap_num = bk_wlan_get_scan_ap_result_numbers();
    if (scan_rst_ap_num == 0)
    {
        *scan_result = NULL;
        return 0;
    }
    scan_rst_table = (struct sta_scan_res *)malloc(sizeof(struct sta_scan_res) * scan_rst_ap_num);
    if (scan_rst_table == NULL)
    {
        *scan_result = NULL;
        return 0;
    }
    bk_wlan_get_scan_ap_result(scan_rst_table, scan_rst_ap_num);
    *scan_result =  scan_rst_table;
    return scan_rst_ap_num;
}

bool_t __attribute__((weak)) FnNetDevEventHookEvent(struct NetDev* iface,enum NetDevEvent event)
{
    switch(event)
     {
         case EN_NETDEVEVENT_IPGET:
             mhdr_set_station_status(6/*MSG_GOT_IP*/); //for rf power save;
             break;
         default:
             break;
     }
     return true;

}


// =============================================================================
// 功能：GMAC网卡和DJYIP驱动初始化函数
// 参数：para
// 返回值  ：true成功  false失败。
// =============================================================================
bool_t ModuleInstall_Wifi(const char *devname, u8 *macaddress,\
                          bool_t loop,u32 loopcycle,\
                          bool_t (*rcvHook)(u8 *buf, u16 len))
{
    tagMacDriver   *pDrive = &gMacDriver;
    struct NetDevPara   devpara;

    memset((void *)pDrive,0,sizeof(tagMacDriver));
    //copy the config para to the pDrive
    memcpy(pDrive->devname,devname,CN_DEVNAME_LEN-1);
    memcpy((void *)pDrive->macaddr,macaddress,CN_MACADDR_LEN);
    if(loop)
    {
        pDrive->loop = 1;
    }
    pDrive->loopcycle = loopcycle;
    pDrive->fnrcvhook = rcvHook;
    //set the work mode and speed
    pDrive->macstat.pm = 0;         //primmiscuous mode:= 1,no MAC addr filte
    pDrive->macstat.duplex = 1;    //duplex full
    pDrive->macstat.speed = 1;     //100 Mb
    pDrive->macstat.bfd = 0;       //broadcast frames disable
    pDrive->macstat.mii = 0;       //use RMII mode

//    mem_init();
    djytimer_init();
    memp_init();
    pbuf_init();
    rwnxl_init();
    rl_init();
    wifi_start();
    //all the configuration has set in the pDrive now,we need some sys assistant
    //application some semphore and mutex
    pDrive->sendsync = Lock_SempCreate(1,1,CN_BLOCK_FIFO,NULL);
    if(NULL == pDrive->sendsync)
    {
        goto RCVSYNC_FAILED;
    }
    pDrive->protect = Lock_MutexCreate(NULL);
    if(NULL == pDrive->protect)
    {
        goto DEVPROTECT_FAILED;
    }

    //install the net device
    devpara.ifctrl = MacCtrl;
    devpara.ifsend = __MacSnd;
    devpara.iftype = EN_LINK_ETHERNET;
    devpara.devfunc = CN_IPDEV_NONE;
    memcpy(devpara.mac,macaddress,6);
    devpara.name = (char *)pDrive->devname;
    devpara.mtu = CN_ETH_MTU;
    devpara.Private = (ptu32_t)pDrive;
    pDrive->devhandle = NetDevInstall(&devpara);
    if(NULL == pDrive->devhandle)
    {
        goto NetInstallFailed;
    }

    NetDevRegisterEventHook(pDrive->devhandle, FnNetDevEventHookEvent);
    info_printf("eth","%s:Install Net Device %s success\n\r",__FUNCTION__,devname);
    return true;

RcvTaskFailed:
    NetDevUninstall(devname);
NetInstallFailed:
    Lock_MutexDelete(pDrive->protect);
    pDrive->protect = NULL;
DEVPROTECT_FAILED:
    Lock_SempDelete(pDrive->sendsync);
    pDrive->sendsync = NULL;
RCVSYNC_FAILED:
    error_printf("bspETH","Install Net Device %s failed\n\r",devname);
    return false;
}


