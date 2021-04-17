/*
 * djynetif.c
 *
 *  Created on: 2019Äê7ÔÂ26ÈÕ
 *      Author: cc
 */

#ifndef DJY_WIFI_CONFIG_H_
#define DJY_WIFI_CONFIG_H_
#include "stddef.h"
#ifdef  __cplusplus
extern "C"{
#endif

typedef  u8     u8_t;
typedef  u16    u16_t;
typedef  u32    u32_t;
typedef   s8      s8_t;
typedef s8_t err_t;

typedef struct {
    u32 addr;
}ip4_addr_t;

typedef ip4_addr_t ip_addr_t;

#define PP_HTONS(x) ((((x) & 0x00ffUL) << 8) | (((x) & 0xff00UL) >> 8))

#define pbuf_init()

/**
 * @ingroup pbuf
 * Enumeration of pbuf types
 */
typedef enum {
  /** pbuf data is stored in RAM, used for TX mostly, struct pbuf and its payload
      are allocated in one piece of contiguous memory (so the first payload byte
      can be calculated from struct pbuf).
      pbuf_alloc() allocates PBUF_RAM pbufs as unchained pbufs (although that might
      change in future versions).
      This should be used for all OUTGOING packets (TX).*/
  PBUF_RAM,
  /** pbuf data is stored in ROM, i.e. struct pbuf and its payload are located in
      totally different memory areas. Since it points to ROM, payload does not
      have to be copied when queued for transmission. */
  PBUF_ROM,
  /** pbuf comes from the pbuf pool. Much like PBUF_ROM but payload might change
      so it has to be duplicated when queued before transmitting, depending on
      who has a 'ref' to it. */
  PBUF_REF,
  /** pbuf payload refers to RAM. This one comes from a pool and should be used
      for RX. Payload can be chained (scatter-gather RX) but like PBUF_RAM, struct
      pbuf and its payload are allocated in one piece of contiguous memory (so
      the first payload byte can be calculated from struct pbuf).
      Don't use this for TX, if the pool becomes empty e.g. because of TCP queuing,
      you are unable to receive TCP acks! */
  PBUF_POOL
} pbuf_type;

/**
 * @ingroup pbuf
 * Enumeration of pbuf layers
 */
typedef enum {
  /** Includes spare room for transport layer header, e.g. UDP header.
   * Use this if you intend to pass the pbuf to functions like udp_send().
   */
  PBUF_TRANSPORT,
  /** Includes spare room for IP header.
   * Use this if you intend to pass the pbuf to functions like raw_send().
   */
  PBUF_IP,
  /** Includes spare room for link layer header (ethernet header).
   * Use this if you intend to pass the pbuf to functions like ethernet_output().
   * @see PBUF_LINK_HLEN
   */
  PBUF_LINK,
  /** Includes spare room for additional encapsulation header before ethernet
   * headers (e.g. 802.11).
   * Use this if you intend to pass the pbuf to functions like netif->linkoutput().
   * @see PBUF_LINK_ENCAPSULATION_HLEN
   */
  PBUF_RAW_TX,
  /** Use this for input packets in a netif driver when calling netif->input()
   * in the most common case - ethernet-layer netif driver. */
  PBUF_RAW
} pbuf_layer;

/** Main packet buffer struct */
struct pbuf {
  /** next pbuf in singly linked pbuf chain */
  struct pbuf *next;

  /** pointer to the actual data in the buffer */
  void *payload;

  /**
   * total length of this buffer and all next buffers in chain
   * belonging to the same packet.
   *
   * For non-queue packet chains this is the invariant:
   * p->tot_len == p->len + (p->next? p->next->tot_len: 0)
   */
  u16_t tot_len;

  /** length of this buffer */
  u16_t len;

  /** pbuf_type as u8_t instead of enum to save space */
  u8_t /*pbuf_type*/ type;

  /** misc flags */
  u8_t flags;

  /**
   * the reference count always equals the number of pointers
   * that refer to this pbuf. This can be pointers from an application,
   * the stack itself, or pbuf->next pointers from a chain.
   */
  u16_t ref;
};


/** This data structure represents an IPv4 address */
struct ipv4_config {
    /** DHCP_Disable DHCP_Client DHCP_Server */
    unsigned addr_type;
    /** The system's IP address in network order. */
    unsigned address;
    /** The system's default gateway in network order. */
    unsigned gw;
    /** The system's subnet mask in network order. */
    unsigned netmask;
    /** The system's primary dns server in network order. */
    unsigned dns1;
    /** The system's secondary dns server in network order. */
    unsigned dns2;
};
/** Network IP configuration.
 *
 *  This data structure represents the network IP configuration
 *  for IPv4 as well as IPv6 addresses
 */
struct wlan_ip_config {
    /** The network IPv4 address configuration that should be
     * associated with this interface. */
    struct ipv4_config ipv4;
};


#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifndef CONTAINER_OF
#define CONTAINER_OF(ptr, type, member)    ((type *)( (char *)ptr - offsetof(type,member) ))
#endif

#ifndef PBUF_LINK_ENCAPSULATION_HLEN
#define PBUF_LINK_ENCAPSULATION_HLEN   0U
#endif

#ifndef PBUF_LINK_HLEN
#define PBUF_LINK_HLEN   (14)
#endif

#ifndef PBUF_IP_HLEN
#define PBUF_IP_HLEN        20
#endif

#ifndef PBUF_TRANSPORT_HLEN
#define PBUF_TRANSPORT_HLEN 20
#endif

#ifndef MEM_ALIGNMENT
#define MEM_ALIGNMENT 4
#endif

#ifndef PBUF_MEM_ALIGN_SIZE
#define PBUF_MEM_ALIGN_SIZE(size) ((unsigned int)((size) + MEM_ALIGNMENT - 1U) & ~(MEM_ALIGNMENT-1U))
#endif

#ifndef LWIP_IPV4
#define LWIP_IPV4 1
#endif
#ifndef LWIP_NETIF_STATUS_CALLBACK
#define LWIP_NETIF_STATUS_CALLBACK  1
#endif

#ifndef NETIF_MAX_HWADDR_LEN
#define NETIF_MAX_HWADDR_LEN 6U
#endif



#ifndef LWIP_NUM_NETIF_CLIENT_DATA
#define LWIP_NUM_NETIF_CLIENT_DATA            0
#endif
enum lwip_internal_netif_client_data_index
{
   LWIP_NETIF_CLIENT_DATA_INDEX_DHCP,
   LWIP_NETIF_CLIENT_DATA_INDEX_IGMP,
   LWIP_NETIF_CLIENT_DATA_INDEX_MAX
};

struct netif;
enum netif_mac_filter_action;
typedef void (*netif_status_callback_fn)(struct netif *netif);
typedef err_t (*netif_igmp_mac_filter_fn)(struct netif *netif,
       const ip4_addr_t *group, enum netif_mac_filter_action action);
typedef err_t (*netif_linkoutput_fn)(struct netif *netif, struct pbuf *p);
typedef err_t (*netif_input_fn)(struct pbuf *p, struct netif *inp);
typedef err_t (*netif_output_fn)(struct netif *netif, struct pbuf *p,
       const ip4_addr_t *ipaddr);

/** Generic data structure used for all lwIP network interfaces.
 *  The following fields should be filled in by the initialization
 *  function for the device driver: hwaddr_len, hwaddr[], mtu, flags */
struct netif {
  /** pointer to next in linked list */
  struct netif *next;

  /** IP address configuration in network byte order */
  ip_addr_t ip_addr;
  ip_addr_t netmask;
  ip_addr_t gw;
  /** This function is called by the network device driver
   *  to pass a packet up the TCP/IP stack. */
  netif_input_fn input;
  /** This function is called by the IP module when it wants
   *  to send a packet on the interface. This function typically
   *  first resolves the hardware address, then sends the packet.
   *  For ethernet physical layer, this is usually etharp_output() */
  netif_output_fn output;
  /** This function is called by ethernet_output() when it wants
   *  to send a packet on the interface. This function outputs
   *  the pbuf as-is on the link medium. */
  netif_linkoutput_fn linkoutput;
  /** This function is called when the netif state is set to up or down
   */
  netif_status_callback_fn status_callback;
  /** This field can be set by the device driver and could point
   *  to state information for the device. */
  void *state;
  void* client_data[LWIP_NETIF_CLIENT_DATA_INDEX_MAX + LWIP_NUM_NETIF_CLIENT_DATA];
  /** Number of Router Solicitation messages that remain to be sent. */
  u8_t rs_count;
  /* the hostname for this netif, NULL is a valid value */
  const char*  hostname;
  /** maximum transfer unit (in bytes) */
  u16_t mtu;
  /** number of bytes used in hwaddr */
  u8_t hwaddr_len;
  /** link level hardware address of this interface */
  u8_t hwaddr[NETIF_MAX_HWADDR_LEN];
  /** flags (@see @ref netif_flags) */
  u8_t flags;
  /** descriptive abbreviation */
  char name[2];
  /** number of this interface */
  u8_t num;
  /** This function could be called to add or delete an entry in the multicast
      filter table of the ethernet MAC.*/
  netif_igmp_mac_filter_fn igmp_mac_filter;
  /* List of packets to be queued for ourselves. */
  struct pbuf *loop_first;
  struct pbuf *loop_last;
  u16_t loop_cnt_current;
};

struct pbuf *pbuf_alloc(pbuf_layer layer, u16_t length, pbuf_type type);
err_t pbuf_copy(struct pbuf *p_to, const struct pbuf *p_from);
u8_t pbuf_free(struct pbuf *p);
struct pbuf *pbuf_coalesce(struct pbuf *p, pbuf_layer layer);
void pbuf_chain(struct pbuf *head, struct pbuf *tail);
struct pbuf *pbuf_dechain(struct pbuf *p);
int ip4addr_aton(const char *cp, ip4_addr_t *addr);
void uap_ip_start();
void uap_ip_down();
void sta_ip_down();
void sta_ip_start();
#ifdef  __cplusplus
}
#endif

#endif /* APP_INCLUDE_DJYNETIF_C_ */
