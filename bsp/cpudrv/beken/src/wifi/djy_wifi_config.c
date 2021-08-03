/*
 * djyipif.c
 *
 *  Created on: 2019年7月20日
 *      Author: cc
 */
//文件说明：博通的 SDK 包与lwip绑定，直接使用lwip的包管理，移植到djyos时，就需要在djyos
//中实现lwip的包管理。

#include <stdint.h>
#include "djy_wifi_config.h"
#include <project_config.h>
#include "netbsp.h"
#include "dbug.h"
#include "net/arpa/inet.h"
#include "rw_pub.h"
#include "ctype.h"
#include "cpu_peri.h"
//#include "sk_intf.h"
#include "component_config_wifi.h"

#define PKG_PBUF   0

#if PKG_PBUF
struct djypbuf
{
    struct NetPkg * npkg;
    struct pbuf pbuf;
};

static struct NetPkg * pbuf2pkg(struct pbuf * p)
{
    struct djypbuf* djypbuf;
    if(p == NULL)
        return NULL;
    djypbuf  =  CONTAINER_OF(p,struct djypbuf,pbuf);
    return djypbuf->npkg;
}

static struct pbuf * pkg2pbuf(struct NetPkg * pkg)
{
    struct djypbuf* mypbuf;
    u8 *p;

    if (pkg == NULL)
        return NULL;
    p =  PkgGetBuffer(pkg);
    mypbuf = (struct djypbuf*)PBUF_MEM_ALIGN_SIZE(p);
    return &mypbuf->pbuf;
}

static u8 djy_pbuf_free(struct pbuf *p)
{
    struct NetPkg * pkg;
    pkg = pbuf2pkg(p);
    PkgTryFreePart(pkg);
    return 0;
}

//func/rwnx_intf/rw_msdu.c  cpu_peri_wifi.c beken378\ip\umac\src\rxu/rxu_cntrl.c
struct pbuf *pbuf_alloc(pbuf_layer layer, u16 length, pbuf_type type)
{
    struct NetPkg  *npkg;
    struct djypbuf* mypbuf;
    struct pbuf* pbuf;

    u16 offset,pak_len;
    /* determine header offset */
    if((layer!=PBUF_RAW)||(type!=PBUF_RAM))
    {
        printf("error : pbuf_alloc layer or type!!\n\r");
        return NULL;
    }
    offset = 0;
    pak_len = PBUF_MEM_ALIGN_SIZE(length) + PBUF_MEM_ALIGN_SIZE(offset + sizeof(struct djypbuf));
    if((npkg =  PkgMalloc(pak_len,CN_PKLGLST_END)) == NULL)
    {
        return NULL;
    }
    pbuf = pkg2pbuf(npkg);
    mypbuf = CONTAINER_OF(pbuf,struct djypbuf,pbuf);

    mypbuf->npkg = npkg;
    mypbuf->pbuf.next  = NULL;
    mypbuf->pbuf.payload = (void*)PBUF_MEM_ALIGN_SIZE(mypbuf + sizeof(struct djypbuf));
    mypbuf->pbuf.tot_len = length;
    mypbuf->pbuf.len = length;
    mypbuf->pbuf.type = type;
    mypbuf->pbuf.flags = 0;
    mypbuf->pbuf.ref = 1;

    return pbuf;
}

#else
struct pbuf *pbuf_alloc(pbuf_layer layer, u16 length, pbuf_type type)
{

    struct pbuf* pbuf;
    u16 offset,pak_len;
    //layer：只需要 PBUF_RAW 层，其他层不需要支持
    if((layer != PBUF_RAW)||(type != PBUF_RAM))
    {
        printf("error : pbuf_alloc layer or type!!\n\r");
        return NULL;
    }
    offset = 0;

    pak_len =  PBUF_MEM_ALIGN_SIZE(length) + PBUF_MEM_ALIGN_SIZE(sizeof(struct pbuf)+offset);
    if((pbuf =  M_Malloc(pak_len,CN_TIMEOUT_FOREVER)) == NULL)
    {
        printf("error :pbuf malloc event %d\n\r",pak_len);
        return NULL;
    }
    pbuf->next  = NULL;
    pbuf->payload = (void*)PBUF_MEM_ALIGN_SIZE((u32)pbuf + offset + sizeof(struct pbuf));
    pbuf->tot_len = length;
    pbuf->len = length;
    pbuf->type = type;
    pbuf->flags = 0;
    pbuf->ref = 1;
    return pbuf;
}
#endif

u8 pbuf_free(struct pbuf *p)
{
    u16 type;
    struct pbuf *q;
    u8 count;

    if (p == NULL) {
      return 0;
    }
    count = 0;
    while (p != NULL) {
      u16 ref;
      ref = --(p->ref);
      if (ref == 0) {
        q = p->next;
        type = p->type;
        if ((type == PBUF_POOL)||(type == PBUF_ROM) || (type == PBUF_REF)) {
            printf("error : pbuf type error\n\r");
        } else {
            #if PKG_PBUF
              djy_pbuf_free(p);
            #else
              free(p);
            #endif
        }
        count++;
        p = q;
      } else {
        p = NULL;
      }
    }
    return count;
}


err_t pbuf_copy(struct pbuf *p_to, const struct pbuf *p_from)
{
    u16 offset_to=0, offset_from=0, len;

    /* is the target big enough to hold the source? */
    if((p_to == NULL) || (p_from == NULL) || (p_to->tot_len < p_from->tot_len))
    {
        printf("error:pbuf_copy: target not big enough to hold source" );
        return (-16);
    }

    /* iterate through pbuf chain */
    do
    {
      /* copy one part of the original chain */
      if ((p_to->len - offset_to) >= (p_from->len - offset_from)) {
        /* complete current p_from fits into current p_to */
        len = p_from->len - offset_from;
      } else {
        /* current p_from does not fit into current p_to */
        len = p_to->len - offset_to;
      }
      memcpy((u8*)p_to->payload + offset_to, (u8*)p_from->payload + offset_from, len);
      offset_to += len;
      offset_from += len;
      if (offset_from >= p_from->len) {
        /* on to next p_from (if any) */
        offset_from = 0;
        p_from = p_from->next;
      }
      if (offset_to == p_to->len) {
        /* on to next p_to (if any) */
        offset_to = 0;
        p_to = p_to->next;
        if((p_to == NULL) && (p_from != NULL))
        {
            printf("error:pbuf_copy: (p_to == NULL) && (p_from != NULL)" );
            return (-16);
        }
      }

      if ((p_from != NULL) && (p_from->len == p_from->tot_len)) {
        /* don't copy more than one packet! */
          if(!(p_from->next == NULL))
          {
            printf("error:pbuf_copy() does not allow packet queues!");
            return -6;
          }
      }
      if ((p_to != NULL) && (p_to->len == p_to->tot_len)) {
        /* don't copy more than one packet! */
          if(!(p_to->next == NULL))
          {
            printf("error:pbuf_copy() does not allow packet queues!");
            return -6;
          }
      }
    } while (p_from);
    return 0;
}




//bk7251drv/app/app.c
struct pbuf *pbuf_coalesce(struct pbuf *p, pbuf_layer layer)
{
    struct pbuf *q;
    err_t err;
    if (p->next == NULL)
    {
      return p;
    }
    q = pbuf_alloc(layer, p->tot_len, PBUF_RAM);
    if (q == NULL) {
      return p;
    }
    err = pbuf_copy(q, p);
    pbuf_free(p);
    if(err)
        debug_printf("djyipif","error:%d\n\r",err);
    return q;
}


//beken378\ip\umac\src\rxu/rxu_cntrl.c
void pbuf_chain(struct pbuf *head, struct pbuf *tail)
{
    struct pbuf *p;

    if((head == NULL) || (tail == NULL))
    {
        printf("(h != NULL) && (t != NULL) (programmer violates API)");
        return;
    }
    /* proceed to last pbuf of chain */
    for (p = head; p->next != NULL; p = p->next) {
      /* add total length of second chain to all totals of first chain */
      p->tot_len += tail->tot_len;
    }
    p->tot_len += tail->tot_len;
    /* chain last pbuf of head (p) with first of tail (t) */
    p->next = tail;
    /* p->next now references t, but the caller will drop its reference to t,
     * so netto there is no change to the reference count of t.
     */
    tail->ref++;
#if PKG_PBUF
    h_pkg = pbuf2pkg(head);
    t_pkg = pbuf2pkg(tail);
    PkgSetNextUnit(h_pkg,t_pkg);
#endif

}



//beken378\ip\umac\src\rxu/rxu_cntrl.c
struct pbuf *pbuf_dechain(struct pbuf *p)
{
    struct pbuf *q;
    u8 tail_gone = 1;
    /* tail */
    q = p->next;
    /* pbuf has successor in chain? */
    if (q != NULL) {
      /* assert tot_len invariant: (p->tot_len == p->len + (p->next? p->next->tot_len: 0) */
      if(!(q->tot_len == p->tot_len - p->len))
          printf("p->tot_len == p->len + q->tot_len\n\r");
      /* enforce invariant if assertion is disabled */
      q->tot_len = p->tot_len - p->len;
      /* decouple pbuf from remainder */
      p->next = NULL;
      /* total length of pbuf p is its own length only */
      p->tot_len = p->len;
      /* q is no longer referenced by p, free it */
      tail_gone = pbuf_free(q);
      if (tail_gone > 0) {
        printf("pbuf_dechain: deallocated %p (as it is no longer referenced)\n", (void *)q);
      }
      /* return remaining tail or NULL if deallocated */
    }
    /* assert tot_len invariant: (p->tot_len == p->len + (p->next? p->next->tot_len: 0) */
    if(!(p->tot_len == p->len))
        printf("p->tot_len == p->len\n\r");
    return ((tail_gone > 0) ? NULL : q);
}
//=======================pbuf=================================================
//============lwip 实现的函数======================================
//wifi/cpu_peri_wifi.c
void memp_init(void)
{
}

#ifndef SIZEOF_ETH_HDR
#define SIZEOF_ETH_HDR   14
#endif
enum eth_type {
  /** Internet protocol v4 */
  ETHTYPE_IP        = 0x0800U,
  /** Address resolution protocol */
  ETHTYPE_ARP       = 0x0806U,
  /** Wake on lan */
  ETHTYPE_WOL       = 0x0842U,
  /** RARP */
  ETHTYPE_RARP      = 0x8035U,
  /** Virtual local area network */
  ETHTYPE_VLAN      = 0x8100U,
  /** Internet protocol v6 */
  ETHTYPE_IPV6      = 0x86DDU,
  /** PPP Over Ethernet Discovery Stage */
  ETHTYPE_PPPOEDISC = 0x8863U,
  /** PPP Over Ethernet Session Stage */
  ETHTYPE_PPPOE     = 0x8864U,
  /** Jumbo Frames */
  ETHTYPE_JUMBO     = 0x8870U,
  ETHTYPE_EAPOL     = 0x888EU,
  /** Process field network */
  ETHTYPE_PROFINET  = 0x8892U,
  /** Ethernet for control automation technology */
  ETHTYPE_ETHERCAT  = 0x88A4U,
  /** Link layer discovery protocol */
  ETHTYPE_LLDP      = 0x88CCU,
  /** Serial real-time communication system */
  ETHTYPE_SERCOS    = 0x88CDU,
  /** Media redundancy protocol */
  ETHTYPE_MRP       = 0x88E3U,
  /** Precision time protocol */
  ETHTYPE_PTP       = 0x88F7U,
  /** Q-in-Q, 802.1ad */
  ETHTYPE_QINQ      = 0x9100U
};

#ifndef ETH_HWADDR_LEN
#define ETH_HWADDR_LEN 6
#endif
struct eth_addr {
  u8 addr[ETH_HWADDR_LEN];
}  __attribute__((packed));
struct eth_hdr {
  struct eth_addr dest;
  struct eth_addr src;
  u16 type;
} __attribute__((packed));


//func/rwnx_intf/rw_msdu.c
void ethernetif_input(s32 iface, struct pbuf *p)
{
    struct eth_hdr *ethhdr;
    struct netif *netif;
    if (p->len <= SIZEOF_ETH_HDR) {
        pbuf_free(p);
        return;
    }

    netif = rwm_mgmt_get_vif2netif((uint8_t)iface);
    if(!netif) {
        //ETH_INTF_PRT("ethernetif_input no netif found %d\r\n", iface);
        pbuf_free(p);
        p = NULL;
        return;
    }

    /* points to packet payload, which starts with an Ethernet header */
    ethhdr = p->payload;

    switch (htons(ethhdr->type))
    {
        /* IP or ARP packet? */
    case ETHTYPE_IP:
    case ETHTYPE_ARP:
    case ETHTYPE_PPPOEDISC:
    case ETHTYPE_PPPOE:
        MacRcv(p);
        break;

    case ETHTYPE_EAPOL:
        ke_l2_packet_tx(p->payload, p->len, iface);
        break;

    default:
        break;
    }
    pbuf_free(p);
    p = NULL;

}

struct interface {
    struct netif netif;
    ip_addr_t ipaddr;
    ip_addr_t nmask;
    ip_addr_t gw;
};
struct interface g_uap;
struct interface g_mlan;
//wlan_ui.c
void net_wlan_add_netif(void *mac)
{
    VIF_INF_PTR vif_entry = NULL;
    struct interface *wlan_if = NULL;
    u8 vif_idx;
    u8 *b = (u8*)mac;
    if(!b || (!(b[0] | b[1] | b[2] | b[3] | b[4] | b[5])))
        return;

    vif_idx = rwm_mgmt_vif_mac2idx(mac);
    if(vif_idx == 0xff) {
        os_printf("net_wlan_add_netif not vif idx found\r\n");
        return ;
    }
    vif_entry = rwm_mgmt_vif_idx2ptr(vif_idx);
    if(!vif_entry) {
        os_printf("net_wlan_add_netif not vif found, %d\r\n", vif_idx);
        return ;
    }

    if(vif_entry->type == VIF_AP) {
        wlan_if = &g_uap;
    } else if(vif_entry->type == VIF_STA) {
        wlan_if = &g_mlan;
    } else {
        os_printf("net_wlan_add_netif with other role\r\n");
        return ;
    }

    wlan_if->ipaddr.addr = INADDR_ANY;

    vif_entry->priv = &wlan_if->netif;

    os_printf("net_wlan_add_netif done!, vif_idx:%d\r\n", vif_idx);

}


//wlan_ui.c
void net_wlan_remove_netif(void *mac)
{
    u8 vif_idx;
    VIF_INF_PTR vif_entry = NULL;
    struct netif *netif = NULL;
    u8 *b = (u8*)mac;
    if(!b || (!(b[0] | b[1] | b[2] | b[3] | b[4] | b[5])))
        return;

    vif_idx = rwm_mgmt_vif_mac2idx(mac);
    if(vif_idx == 0xff) {
        os_printf("net_wlan_add_netif not vif idx found\r\n");
        return ;
    }
    vif_entry = rwm_mgmt_vif_idx2ptr(vif_idx);
    if(!vif_entry) {
        os_printf("net_wlan_add_netif not vif found, %d\r\n", vif_idx);
        return ;
    }

    netif = (struct netif *)vif_entry->priv;
    if(!netif) {
        os_printf("net_wlan_remove_netif netif is null\r\n");
        return;
    }

//    err = netifapi_netif_remove(netif);
//    if(err != ERR_OK) {
//        os_printf("net_wlan_remove_netif failed\r\n");
//    } else {
//        netif->state = NULL;
//    }
    netif->state = NULL;

    os_printf("net_wlan_remove_netif done!, vif_idx:%d\r\n", vif_idx);
}


tagRouterPara sta_ip_settings;
tagRouterPara uap_ip_settings;
u32 sta_ip_hop,sta_ip_subnet,sta_ip_ip,sta_ip_submask,sta_ip_dns,sta_ip_dnsbak;
u32 uap_ip_hop,uap_ip_subnet,uap_ip_ip,uap_ip_submask,uap_ip_dnss,uap_ip_dnsbak;

//wlan_ui.c
void ip_address_set(s32 iface, s32 dhcp, char *ip, char *mask, char *gw, char*dns)
{
    if (dhcp == 1)
    {

    }
    else
    {

        //memset(&para,0,sizeof(para));
        sta_ip_ip      = inet_addr(ip);
        sta_ip_submask = inet_addr(mask);
        sta_ip_hop     = inet_addr(gw);
        sta_ip_dns     = inet_addr(dns);
        sta_ip_dnsbak  = inet_addr(dns);
        if (sta_ip_submask == INADDR_BROAD)
            sta_ip_submask = 0x00FFFFFF;// if not set valid netmask, set as 255.255.255.0
        sta_ip_subnet = sta_ip_ip & sta_ip_submask;
        if (iface == 1) // Station
        {
            sta_ip_settings.ver = EN_IPV_4;
            sta_ip_settings.ifname = CFG_WIFI_DEV_NAME;
            sta_ip_settings.mask = &sta_ip_submask;
            sta_ip_settings.net = &sta_ip_subnet;
            sta_ip_settings.host = &sta_ip_ip;
            sta_ip_settings.hop = &sta_ip_hop;
            sta_ip_settings.dns = &sta_ip_dns;
            sta_ip_settings.dnsbak = &sta_ip_dnsbak;
            sta_ip_settings.prior = CN_ROUT_PRIOR_UNI;
            sta_ip_settings.flags = 0;
        }
        else
        {
            uap_ip_settings.ver = EN_IPV_4;
            uap_ip_settings.ifname = CFG_WIFI_DEV_NAME;
            uap_ip_settings.mask = &sta_ip_submask;
            uap_ip_settings.net = &sta_ip_subnet;
            uap_ip_settings.host = &sta_ip_ip;
            uap_ip_settings.hop = &sta_ip_hop;
            uap_ip_settings.dns = &sta_ip_dns;
            uap_ip_settings.dnsbak = &sta_ip_dnsbak;
            uap_ip_settings.prior = CN_ROUT_PRIOR_UNI;
            uap_ip_settings.flags = 0;
        }
    }
}

//wlan_ui.c
void uap_ip_start()
{
//    RouterCreate(&uap_ip_settings);
}
//wlan_ui.c
void uap_ip_down()
{
    //清除资源
//    RouterRemove(&uap_ip_settings);

}
//wlan_ui.c
void sta_ip_down()
{
//    RouterRemove(&sta_ip_settings);
}
//wlan_ui.c
void sta_ip_start()
{
//    RouterCreate(&sta_ip_settings);
}

//firmware/bk7251drv/app/app.c
uint8_t dhcp_start_flag = 0;




