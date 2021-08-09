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
#include <net/arpa/inet.h>
#include <dbug.h>
#include "param_config.h"
#include "shell.h"
#include "app.h"
#include "rwnx.h"
#include "role_launch.h"
#include "djytimer.h"
#include "djy_wifi_config.h"
#include "ieee802_11_demo.h"
#include "rw_pub.h"
#include "wlan_ui_pub.h"
#include "component_config_wifi.h"

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
static bool_t __MacSnd(struct NetDev *handle,struct NetPkg * pkg,u32 netdevtask)
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
                #if(CN_BEKEN_SDK_V3 == 1)
                ret = rtos_push_to_queue(&g_wifi_core.io_queue, &msg, 1 * SECONDS);
                #else
                ret = bk_rtos_push_to_queue(&g_wifi_core.io_queue, &msg, 1 * SECONDS);
                #endif
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
#if (CN_BEKEN_SDK_V3 == 1)
    bk_wlan_stop(BK_STATION);
#else
    bk_wlan_stop(STATION);
#endif
    DJY_EventDelay(300*mS);//加个延时，如果断开立即快连，有时发不出去数据。
}

void DjyWifi_ApOpen(char *ap_ssid, char *ap_key)
{
    demo_softap_app_init(ap_ssid,ap_key);
}

void DjyWifi_ApClose(void)
{
#if (CN_BEKEN_SDK_V3 == 1)
    bk_wlan_stop(BK_SOFT_AP);
#else
    bk_wlan_stop(SOFT_AP);
#endif
}

extern int wpa_get_psk(char *psk);
extern int wpa_get_passphrase_md5(unsigned char *md5_passphrase, int len);
void DjyWifi_StaConnectDone(void)
{
#if (CN_BEKEN_SDK_V3 == 1)
    //新代码不用写入，在回调里有写入信息
#else
    LinkStatusTypeDef link_status;
    struct wlan_fast_connect ap_info;
    uint8_t len = 0;
#if (CN_BEKEN_SDK_V3 == 1)
    if ((bk_wlan_get_link_status(&link_status) == kNoErr) && (BK_SECURITY_TYPE_WEP != link_status.security))
#else
    if ((bk_wlan_get_link_status(&link_status) == kNoErr) && (SECURITY_TYPE_WEP != link_status.security))
#endif
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
#endif
}

int GetConnectedRssiValue()
{
    LinkStatusTypeDef link_status;
    int ret = 0;

    if (bk_wlan_get_link_status(&link_status) == kNoErr) {
#if (CN_BEKEN_SDK_V3 == 1)
        if ((link_status.conn_state & RW_EVT_STA_GOT_IP) || (link_status.conn_state & RW_EVT_STA_CONNECTED )) {
#else
        if ((link_status.conn_state & MSG_GOT_IP) || (link_status.conn_state & MSG_CONN_SUCCESS )) {
#endif
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
#if (CN_BEKEN_SDK_V3 == 1)
    printf("info: %s, Do Normal WiFi Connect, ssid=%s, connect_key=%s\r\n", __FUNCTION__, ssid, connect_key);
    demo_sta_app_init(ssid, connect_key);
#else
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
    else
    {
        SetQuickConnectFlag(0);
        printf("info: %s, Do Normal WiFi Connect, ssid=%s, connect_key=%s\r\n", __FUNCTION__, ssid, connect_key);
        demo_sta_app_init(ssid, connect_key);
    }
#endif


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

void rl_write_bssid_info(void);//chenws:如果分配IP就写入快连信息
bool_t __attribute__((weak)) FnNetDevEventHookEvent(struct NetDev* iface,enum NetDevEvent event)
{
    switch(event)
     {
        case EN_NETDEVEVENT_IPGET:
#if (CN_BEKEN_SDK_V3 == 1)
             mhdr_set_station_status(RW_EVT_STA_GOT_IP); //for rf power save;
             rl_write_bssid_info();
#else
             mhdr_set_station_status(6/*MSG_GOT_IP*/); //for rf power save;
#endif
             break;
         default:
             break;
     }
     return true;

}

static char   gc_NetMac[CN_MACADDR_LEN] = { 0xC8, 0x47, 0x8C, 0x00, 0x00, 0x00};

//timeout: microsecond
int PendDhcpDone(unsigned int timeout)
{
    return DHCP_WaitDhcpDone(CFG_WIFI_DEV_NAME, timeout);
}

void DhcpStaClearIp(void)
{
    printf("info: DhcpStaClearIp!!!\r\n");
    DhcpclientDeleteAllTask();
}

//默认不指定IP获取DHCP, 需要四个步骤
//__attribute__((weak)) int cb_ip_get(u32 *ip)
//{
//    return 0;
//}
//__attribute__((weak)) int cb_ip_set(u32 ip){
//    return 0;
//}
//int dhcp_setip_cb(const char *ifname, int (*cb_ip_set)(u32 ip));

//------------------------------------------------------------------------------
//功能：启动WiFi的DHCP客户端
//参数：AssignIP，固定分配的IP，一般用于系统重启后，向DHCP服务器重新申请原来使用的IP
//              如果AssignIP == 0，则忽略，否则客户端直接进入收到offer状态
//      cb_ip_set，DHCP客户端获取IP后的回调函数。
//返回：无
//------------------------------------------------------------------------------
void DhcpStaStartIp(u32 AssignIP,s32 (*cb_ip_got)(u32 *ip))
{
    NetDev_DHCP_SET_GotIP_CB(CFG_WIFI_DEV_NAME, cb_ip_got);
//  dhcp_setip_cb(CFG_WIFI_DEV_NAME, cb_ip_set);

    DhcpclientDeleteAllTask();
    if(DHCP_AddClientTask(CFG_WIFI_DEV_NAME, AssignIP))
    {
       printk("%s:Add %s success\r\n",__FUNCTION__,CFG_WIFI_DEV_NAME);
    }
    else
    {
        printk("%s:Add %s failed\r\n",__FUNCTION__,CFG_WIFI_DEV_NAME);
    }
}

u32 trng_get_random(void);

// =============================================================================
// 功能：GMAC网卡和DJYIP驱动初始化函数
// 参数：para
// 返回值  ：true成功  false失败。
// =============================================================================
bool_t ModuleInstall_Wifi(void)
{
    tagMacDriver   *pDrive = &gMacDriver;
    struct NetDevPara   devpara;

    FILE *fd = NULL;
    struct stat file_state;

    wifi_get_mac_address(gc_NetMac,CONFIG_ROLE_NULL);//防止重新设置MAC地址
    memset(&file_state, 0, sizeof(struct stat));
    stat(CFG_MAC_DATA_FILE_NAME,&file_state);
    fd = fopen(CFG_MAC_DATA_FILE_NAME,"a+");
    if(fd)
    {
        fseek(fd, -6, SEEK_END);
        if(fread(gc_NetMac, 1, 6, fd) != 6)
    //  if(File_GetNameValueFs(CFG_MAC_DATA_FILE_NAME, gc_NetMac, 6) == false)
        {
            u32 mac_rand =  trng_get_random();
            memcpy(&gc_NetMac[2], &mac_rand, 4);
            gc_NetMac[0] = 0x00;
            gc_NetMac[1] = 0x01;
#if CFG_MODULE_ENABLE_EASY_FILE_SYSTEM
            if((file_state.st_size + 6) > CFG_EFS_FILE_SIZE_LIMIT)
            {
                if(fclose(fd) == -1)
                {
                    printf("before remove, close file \" %s \" fail\r\n",CFG_MAC_DATA_FILE_NAME);
                }
                else
                {
                    fd = NULL;
                    if(remove(CFG_MAC_DATA_FILE_NAME) == -1)
                        printf("remove file \" %s \" fail\r\n",CFG_MAC_DATA_FILE_NAME);
                    else
                    {
                        fd = fopen(CFG_MAC_DATA_FILE_NAME,"a+");
                        if(fd == NULL)
                            printf("after remove file, open file \" %s \" fail\r\n",CFG_MAC_DATA_FILE_NAME);
                    }
                }
            }
#endif
            if(fd)
            {
                fseek(fd, 0, SEEK_END);
                if(fwrite(gc_NetMac, 1, 6, fd) != 6)
                    printf("write file \" %s \" fail\r\n",CFG_MAC_DATA_FILE_NAME);
            }
    //      File_SetNameValueFs(CFG_MAC_DATA_FILE_NAME, gc_NetMac, 6);
        }
        if(fd)
        {
            if(fclose(fd) == -1)
                printf("close file \" %s \" fail\r\n",CFG_MAC_DATA_FILE_NAME);
        }
    }
    else
        printf("open file \" %s \" fail\r\n",CFG_MAC_DATA_FILE_NAME);
    printf("\r\n==WIFI MAC==:%02X-%02X-%02X-%02X-%02X-%02X!\r\n",
        gc_NetMac[0], gc_NetMac[1], gc_NetMac[2], gc_NetMac[3], gc_NetMac[4], gc_NetMac[5]);

    wifi_set_mac_address((char*)gc_NetMac);


    memset((void *)pDrive,0,sizeof(tagMacDriver));
    //copy the config para to the pDrive
    memcpy(pDrive->devname,CFG_WIFI_DEV_NAME,CN_DEVNAME_LEN-1);
    memcpy((void *)pDrive->macaddr,gc_NetMac,CN_MACADDR_LEN);
    pDrive->loop = 0;
    pDrive->loopcycle = 1*mS;
    pDrive->fnrcvhook = NULL;
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
    app_pre_start( );   //博通原本提供的是app_start函数，但许多开源库中有重名函数
                        //故直接调用app_start内部调用的函数。
//#if ( CN_BEKEN_SDK_V2 == TRUE )
//    wifi_start();
//#else if( CN_BEKEN_SDK_V3 == TRUE )
//    void app_start(void);
//    app_start();
//#endif
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
    memcpy(devpara.mac,gc_NetMac,6);
    devpara.name = (char *)pDrive->devname;
    devpara.mtu = CN_ETH_MTU;
    devpara.Private = (ptu32_t)pDrive;
    pDrive->devhandle = NetDev_Install(&devpara);
    if(NULL == pDrive->devhandle)
    {
        goto NetInstallFailed;
    }

    NetDev_RegisterEventHook(pDrive->devhandle, FnNetDevEventHookEvent);
    info_printf("eth","%s:Install Net Device %s success\n\r",__FUNCTION__,CFG_WIFI_DEV_NAME);
    return true;

NetInstallFailed:
    Lock_MutexDelete(pDrive->protect);
    pDrive->protect = NULL;
DEVPROTECT_FAILED:
    Lock_SempDelete(pDrive->sendsync);
    pDrive->sendsync = NULL;
RCVSYNC_FAILED:
    error_printf("bspETH","Install Net Device %s failed\n\r",CFG_WIFI_DEV_NAME);
    return false;
}

struct NetDev;
void NetDev_SetDefault(struct NetDev *NetDev);
void dhcpd_route_add_default()
{
    u32 hop,subnet,ip,submask,dns,dnsbak;
    tagRouterPara para;
    struct NetDev *NetDev;

    /*设置默认网卡，不然默认网卡都是闭环网卡 */
    NetDev_SetDefault(NetDev_GetHandle(CFG_WIFI_DEV_NAME));
    NetDev = NetDev_GetHandle(CFG_WIFI_DEV_NAME);
    memset(&para,0,sizeof(para));
    ip      = inet_addr(CFG_DHCPD_SERVERIP);
    submask = inet_addr(CFG_DHCPD_NETIP);
    hop     = inet_addr(CFG_DHCPD_ROUTERIP);
    dns     = inet_addr(CFG_DHCPD_DNS     );
    dnsbak  = inet_addr(CFG_DHCPD_DNSBAK);

    subnet = ip & submask;
    para.ver = EN_IPV_4;
    para.ifname = CFG_WIFI_DEV_NAME;
    para.mask = &submask;
    para.net = &subnet;
    para.host = &ip;
//  para.hop = &hop;
//  para.dns = &dns;
//  para.dnsbak = &dnsbak;
    para.prior = CN_ROUT_PRIOR_UNI;
    para.flags = 0;

    NetDev_SetDns(EN_IPV_4,NetDev, &dns, &dnsbak);
    NetDev_SetGateway(EN_IPV_4,NetDev, &hop);
    if(RouterCreate(&para))
    {
        printk("%s CreateRout:%s success\r\n",CFG_WIFI_DEV_NAME,CFG_DHCPD_SERVERIP);
    }
    else
    {
        printk("%s CreateRout:%s failed\r\n",CFG_WIFI_DEV_NAME,CFG_DHCPD_SERVERIP);
    }
}



