
#ifndef DHCP_H_
#define DHCP_H_

typedef enum
{
    EN_DHCP_SET_STATUS_CMD = 0,
    EN_DHCP_GET_STATUS_CMD,
    EN_DHCP_LAST_CMD,
}enDHCPCmd;

typedef enum
{
    EN_DHCP_DISCONNET_STATUS = 0,
    EN_DHCP_CONNET_STATUS,
    EN_DHCP_CMD_ERR_STATUS,
    EN_DHCP_LAST_STATUS,
}enDHCPStatus;

enDHCPStatus DHCP_ConnetStatus(enDHCPCmd cmd,enDHCPStatus status);

#endif
