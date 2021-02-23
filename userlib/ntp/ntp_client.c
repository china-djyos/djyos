#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define NTP_TIMESTAMP_DELTA 2208988800ull

#define LI(packet)   (uint8_t) ((packet.li_vn_mode & 0xC0) >> 6) // (li   & 11 000 000) >> 6
#define VN(packet)   (uint8_t) ((packet.li_vn_mode & 0x38) >> 3) // (vn   & 00 111 000) >> 3
#define MODE(packet) (uint8_t) ((packet.li_vn_mode & 0x07) >> 0) // (mode & 00 000 111) >> 0

  // Structure that defines the 48 byte NTP packet protocol.

struct ntp_packet
{
    uint8_t li_vn_mode;      // Eight bits. li, vn, and mode.
                             // li.   Two bits.   Leap indicator.
                             // vn.   Three bits. Version number of the protocol.
                             // mode. Three bits. Client will pick mode 3 for client.

    uint8_t stratum;         // Eight bits. Stratum level of the local clock.
    uint8_t poll;            // Eight bits. Maximum interval between successive messages.
    uint8_t precision;       // Eight bits. Precision of the local clock.

    uint32_t rootDelay;      // 32 bits. Total round trip delay time.
    uint32_t rootDispersion; // 32 bits. Max error aloud from primary clock source.
    uint32_t refId;          // 32 bits. Reference clock identifier.

    uint32_t refTm_s;        // 32 bits. Reference time-stamp seconds.
    uint32_t refTm_f;        // 32 bits. Reference time-stamp fraction of a second.

    uint32_t origTm_s;       // 32 bits. Originate time-stamp seconds.
    uint32_t origTm_f;       // 32 bits. Originate time-stamp fraction of a second.

    uint32_t rxTm_s;         // 32 bits. Received time-stamp seconds.
    uint32_t rxTm_f;         // 32 bits. Received time-stamp fraction of a second.

    uint32_t txTm_s;         // 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
    uint32_t txTm_f;         // 32 bits. Transmit time-stamp fraction of a second.

}  __attribute__((packed));               // Total: 384 bits or 48 bytes.

struct hostent * gethostbyname_r(const char *name,struct hostent_ext *pnew);
int get_ntp_tiemstamp(u32 *ptimestamp, char *ntp_domain, int timeout_ms)
{
  u32 timestamp = 0;
  int ret = 0;
  int sockfd = -1, n; // Socket file descriptor and the n return result from writing/reading from the socket.
  int portno = 123; // NTP UDP port number.
  char* host_name = "us.pool.ntp.org"; // NTP server host-name.
  if (ntp_domain) {
      host_name = ntp_domain;
  }

  // Create and zero out the packet. All 48 bytes worth.
  struct ntp_packet packet = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  memset(&packet, 0, sizeof(struct ntp_packet));
  // Set the first byte's bits to 00,011,011 for li = 0, vn = 3, and mode = 3. The rest will be left set to zero.

  *((char *) &packet + 0) = 0x1b; // Represents 27 in base 10 or 00011011 in base 2.

  // Create a UDP socket, convert the host-name to an IP address, set the port number,
  // connect to the server, send the packet, and then read in the return packet.

  struct sockaddr_in serv_addr; // Server address data structure.
  struct hostent *server;      // Server data structure.
  sockfd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ); // Create a UDP socket.

  if ( sockfd < 0 ) {
      ret = -1;
      printf( "ERROR opening socket\r\n" );
      goto FUN_RET;
  }

  int opt = 5*1000*mS;  //time out time
  if (timeout_ms > 0) {
      opt = timeout_ms * mS;
  }
  if(0 != setsockopt(sockfd, SOL_SOCKET,SO_RCVTIMEO,&opt,sizeof(opt)))
  {
      ret = -2;
      goto FUN_RET;
  }
  struct hostent_ext *pnew = (struct hostent_ext*)malloc(sizeof(struct hostent_ext));
  if (pnew == 0) {
      printf("error: malloc failed!\r\n");
      ret = -3;
      goto FUN_RET;
  }
  server = gethostbyname_r(host_name, pnew); //Convert URL to IP.
  //server = gethostbyname( host_name ); // Convert URL to IP.
  if ( server == NULL ) {
      ret = -4;
      printf( "ERROR, no such host\r\n" );
      goto FUN_RET;
  }
  // Zero out the server address structure.
  memset((char*)&serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;

  // Copy the server's IP address to the server address structure.

  bcopy( (char*)server->h_addr, ( char* ) &serv_addr.sin_addr.s_addr, server->h_length);

  // Convert the port number integer to network big-endian style and save it to the server address structure.
  serv_addr.sin_port = htons(portno);

  // Call up the server using its IP address and port number.
  if (connect( sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr) ) < 0 ) {
      ret = -5;
      printf( "ERROR connecting\r\n" );
      goto FUN_RET;
  }

  int addrlen = sizeof(struct sockaddr);
  // Send it the NTP packet it wants. If n == -1, it failed.
  n = sendto(sockfd, (char*)&packet, sizeof(struct ntp_packet), 0, (struct sockaddr *)&serv_addr, addrlen);
  if ( n < 0 ) {
      ret = -6;
      printf( "ERROR writing to socket\r\n" );
      goto FUN_RET;
  }

  // Wait and receive the packet back from the server. If n == -1, it failed.
  addrlen = sizeof(struct sockaddr);
  n = recvfrom(sockfd,  (char*)&packet, sizeof(struct ntp_packet), 0, (struct sockaddr *)&serv_addr, &addrlen);

  if ( n < 0 ) {
      ret = -7;
      printf( "warnning: NTP timeout reading from socket, try again!\r\n" );
      goto FUN_RET;
  }
  // These two fields contain the time-stamp seconds as the packet left the NTP server.
  // The number of seconds correspond to the seconds passed since 1900.
  // ntohl() converts the bit/byte order from the network's to host's "endianness".

  packet.txTm_s = ntohl( packet.txTm_s ); // Time-stamp seconds.
  packet.txTm_f = ntohl( packet.txTm_f ); // Time-stamp fraction of a second.

  // Extract the 32 bits that represent the time-stamp seconds (since NTP epoch) from when the packet left the server.
  // Subtract 70 years worth of seconds from the seconds since 1900.
  // This leaves the seconds since the UNIX epoch of 1970.
  // (1900)------------------(1970)**************************************(Time Packet Left the Server)
  timestamp = packet.txTm_s-NTP_TIMESTAMP_DELTA;
  printf("NTP CLIENT GET TIMESTAMP: %u\r\n", timestamp);
  // Print the time we got from the server, accounting for local timezone and conversion from UTC time.
  ret = 0;

FUN_RET:
    if (sockfd>=0) {
        closesocket(sockfd);
    }
    if (pnew) free(pnew);
    *ptimestamp  = timestamp;
    return ret;
}
