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
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���

//%$#@end initcode  ****��ʼ���������
//%$#@describe      ****���������ʼ
//component name:"cpu onchip MAC"//CPU��mac����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"tcpip","lock","int","tcpip"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_MAC == false )
//#warning  " cpu_onchip_MAC  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header    //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_MAC    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,1000,10000,
#define CFG_ETH_LOOP_CYCLE      1000         //"������ѯ��������",
//%$#@enum,true,false,
#define CFG_ETH_LOOP_ENABLE     true        //"���������Ƿ���ѯ",
//%$#@string,1,32,
#define CFG_ETH_DEV_NAME        "BK7251_WiFi"//"��������",
//%$#@string,1,100,
#define CFG_FAST_DATA_FILE_NAME "/efs/fast_data.dat"    //�������Ϣ���ļ�·��
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

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
            case (EN_NETDEV_ADDRFILTER):    //����һ���Ƿ�������а�
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
    DJY_EventDelay(300*mS);//�Ӹ���ʱ������Ͽ�������������ʱ������ȥ���ݡ�
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
        memset(&ap_info, 0, sizeof(struct wlan_fast_connect));//�������
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
// ���ܣ�GMAC������DJYIP������ʼ������
// ������para
// ����ֵ  ��true�ɹ�  falseʧ�ܡ�
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


