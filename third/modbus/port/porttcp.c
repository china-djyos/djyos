#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <multiplex.h>
#include <dbug.h>
#include <sys/socket.h>
#include "port.h"
#include "mb.h"
#include "mbport.h"


static u8 TCP_RecvStack[0x800];
#define MB_TCP_UID          6
#define MB_TCP_LEN          4
#define MB_TCP_FUNC         7

#define MB_TCP_DEFAULT_PORT 502 /* TCP listening port. TCP监听端口*/
#define MB_TCP_READ_TIMEOUT 1000        /* Maximum timeout to wait for packets. 等待报文的最大超时时间*/
#define MB_TCP_READ_CYCLE   100 /* Time between checking for new data.检查新数据的间隔时间。 */

// 7是MBAP报文
#define MB_TCP_BUF_SIZE     ( 256 + 7 ) /* Must hold a complete Modbus TCP frame. */


s32          xListenSocket = INVALID_SOCKET;      //tcp监听句柄
s32          xClientSocket = INVALID_SOCKET;
static struct MultiplexSetsCB *ModbusTCP_ListenSet = NULL;
static struct MultiplexSetsCB *ModbusTCP_ClientSet = NULL;
static UCHAR    aucTCPBuf[MB_TCP_BUF_SIZE];     //接收数据缓存
static USHORT   usTCPBufPos;
static USHORT   usTCPFrameBytesLeft;

static BOOL     prvbMBPortAcceptClient( void );
static void     prvvMBPortReleaseClient( void );

ptu32_t xMBPortTCPPool(void);

BOOL xMBTCPPortInit( USHORT usTCPPort )
{
    USHORT          usPort;
    struct sockaddr_in serveraddr;
    u16 EvttID = 0;

    EvttID = DJY_EvttRegist(EN_INDEPENDENCE, 200, 0, 1, (void *)xMBPortTCPPool, 
                                TCP_RecvStack, sizeof(TCP_RecvStack), "tcp recv");
    if (CN_EVTT_ID_INVALID == EvttID)
    {
        error_printf("modbus", "tcp recv Evtt regist fail\r\n");
    }
    else
    {
        ModbusTCP_ListenSet = Multiplex_Create(1);
        if (NULL == ModbusTCP_ListenSet)
        {
            error_printf("modbus tcp","Create listen multiplex failed!\n\r");
        }
        else
        {
            ModbusTCP_ClientSet = Multiplex_Create(1);
            if (NULL == ModbusTCP_ClientSet)
            {
                error_printf("modbus tcp","Create client multiplex failed!\n\r");
            }
            else
            {
                if( usTCPPort == 0 )
                {   //使用默认端口号
                    usPort = MB_TCP_DEFAULT_PORT;
                }
                else
                {
                    usPort = ( USHORT ) usTCPPort;
                }

                memset( &serveraddr, 0, sizeof( serveraddr ) );
                serveraddr.sin_family = AF_INET;
                serveraddr.sin_addr.s_addr = htonl( INADDR_ANY );
                serveraddr.sin_port = htons( usPort );

                if( ( xListenSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) == -1 )
                {
                    error_printf( "modbus", "Create socket failed.\r\n" );
                }
                else
                {
                    if( bind( xListenSocket, ( struct sockaddr * )&serveraddr, sizeof( serveraddr ) ) == -1 )
                    {
                        error_printf( "modbus", "Bind socket failed.\r\n" );
                    }
                    else 
                    {
                        if( listen( xListenSocket, 5 ) == -1 )     //最多允许五个客户端连接
                        {
                            error_printf( "modbus", "Listen socket failed.\r\n" );
                        }
                        else
                        {
                            if (Multiplex_AddObject(ModbusTCP_ListenSet, xListenSocket, CN_SOCKET_IOACCEPT | CN_MULTIPLEX_SENSINGBIT_ET, 0))
                            {
                                DJY_EventPop(EvttID, NULL, 0, (ptu32_t)0, (ptu32_t)0, 0);

                                return TRUE;
                            }
                            else
                            {
                                error_printf( "modbus", "add xListenSocket to ModbusTCP_ListenSet fail!\n\r");
                            }

                        }
                    }

                    closesocket(xListenSocket);
                    xListenSocket = INVALID_SOCKET;
                }
            }
        }


        DJY_EvttUnregist(EvttID);
        EvttID = CN_EVTT_ID_INVALID;
    }
    return FALSE;


}

void vMBTCPPortClose(  )
{
    // Close all client sockets. //关闭所有客户端套接字
    if( xClientSocket != INVALID_SOCKET )
    {
        prvvMBPortReleaseClient(  );
    }
    // Close the listener socket.
    if( xListenSocket != INVALID_SOCKET )
    {
        closesocket( xListenSocket );     //关闭监听端口
        xListenSocket = INVALID_SOCKET;
    }
}

void vMBTCPPortDisable( void )
{
    /* Close all client sockets. 关闭所有客户端套接字*/
    if( xClientSocket != INVALID_SOCKET )
    {
        prvvMBPortReleaseClient(  );
    }
}

/*! \ingroup port_win32tcp
 *
 * \brief Pool the listening socket and currently connected Modbus TCP clients for new events.
 * \internal
 *
 * This function checks if new clients want to connect or if already connected clients are sending requests. 
 * If a new client is connected and there are still client slots left (The current implementation supports only one) then the connection is accepted and an event object for the new client socket is activated (See prvbMBPortAcceptClient() ).
 * Events for already existing clients in \c FD_READ and \c FD_CLOSE. In case of an \c FD_CLOSE the client connection is released (See prvvMBPortReleaseClient() ).
 * In case of an \c FD_READ command the existing data is read from the client and if a complete frame has been received the Modbus Stack is notified.

 * \return FALSE in case of an internal I/O error. 
 * For example if the internal event objects are in an invalid state. 
 * Note that this does not include any client errors. In all other cases returns TRUE.
 */
// tcp端口轮询
ptu32_t xMBPortTCPPool(void)
{
    s32 TempSocket = INVALID_SOCKET;
    int             ret;
    USHORT          usLength;

    while (1)
    {
        if( xClientSocket == INVALID_SOCKET )
        {
            /* Accept to client */
            TempSocket = Multiplex_Wait(ModbusTCP_ListenSet, NULL, NULL, CN_TIMEOUT_FOREVER);
            if (TempSocket == xListenSocket)
            {
                if (prvbMBPortAcceptClient(  ) == FALSE)
                {
                    error_printf( "modbus tcp", "Accept client fail\r\n" );
                }
            }
        }

        if (xClientSocket != INVALID_SOCKET)
        {
            TempSocket = Multiplex_Wait(ModbusTCP_ClientSet, NULL, NULL, CN_TIMEOUT_FOREVER);
            if (TempSocket == xClientSocket)
            {
                while(1)
                {
                    if( ( ( ret =
                            recv( xClientSocket, &aucTCPBuf[usTCPBufPos], usTCPFrameBytesLeft,
                                0 ) ) == INVALID_SOCKET ))
                    {
                        closesocket( xClientSocket );
                        xClientSocket = INVALID_SOCKET;
                        break;
                    }
                    usTCPBufPos += ret;
                    usTCPFrameBytesLeft -= ret;
                    if( usTCPBufPos >= MB_TCP_FUNC )
                    {
                        // 长度是Modbus PDU(功能码+数据)的字节数和单元标识符。
                        /* Length is a byte count of Modbus PDU (function code + data) and the unit identifier. */
                        usLength = aucTCPBuf[MB_TCP_LEN] << 8U;
                        usLength |= aucTCPBuf[MB_TCP_LEN + 1];

                        /* Is the frame already complete. 帧是否已经完成。*/
                        if( usTCPBufPos < ( MB_TCP_UID + usLength ) )
                        {
                            // 数据没接收完全，计算还剩多少数据需要接收
                            usTCPFrameBytesLeft = usLength + MB_TCP_UID - usTCPBufPos;
                            if ((usTCPFrameBytesLeft + usTCPBufPos) >= MB_TCP_BUF_SIZE)
                            {
                                usTCPFrameBytesLeft = MB_TCP_BUF_SIZE - usTCPBufPos;
                            }
                        }
                        /* The frame is complete. 帧完成。*/
                        else if( usTCPBufPos == ( MB_TCP_UID + usLength ) )
                        {
                            ( void )xMBPortEventPost( EV_FRAME_RECEIVED );
                            break;
                        }
                        /* This can not happend because we always calculate the number of bytes
                        * to receive. */
                        else
                        {
                            if (usTCPBufPos > ( MB_TCP_UID + usLength ))
                            {
                                error_printf( "modbus tcp", "usTCPBufPos(%d) is overflow\r\n", usTCPBufPos);
                                usTCPBufPos = 0;
                                usTCPFrameBytesLeft = MB_TCP_FUNC;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}

/*!
 * \ingroup port_win32tcp
 * \brief Receives parts of a Modbus TCP frame and if complete notifies the protocol stack.
 * \internal 
 *
 * This function reads a complete Modbus TCP frame from the protocol stack.
 * It starts by reading the header with an initial request size for usTCPFrameBytesLeft = MB_TCP_FUNC. 
 * If the header is complete the number of bytes left can be calculated from it (See Length in MBAP header).
 * Further read calls are issued until the frame is complete.
 *
 * \return \c TRUE if part of a Modbus TCP frame could be processed. 
 * In case of a communication error the function returns \c FALSE.
 */

// 从接收数据缓存中拿走数据
BOOL xMBTCPPortGetRequest( UCHAR ** ppucMBTCPFrame, USHORT * usTCPLength )
{
    *ppucMBTCPFrame = &aucTCPBuf[0];
    *usTCPLength = usTCPBufPos;

    /* Reset the buffer. */
    usTCPBufPos = 0;
    usTCPFrameBytesLeft = MB_TCP_FUNC;
    return TRUE;
}

// 通过tcp发送数据
BOOL xMBTCPPortSendResponse( const UCHAR * pucMBTCPFrame, USHORT usTCPLength )
{
    BOOL            bFrameSent = FALSE;
    BOOL            bAbort = FALSE;
    int             res;
    int             iBytesSent = 0;
    int             iTimeOut = MB_TCP_READ_TIMEOUT;

    do
    {
        res = send( xClientSocket, &pucMBTCPFrame[iBytesSent], usTCPLength - iBytesSent, 0 );
        switch ( res )
        {
        case -1:
            if( iTimeOut > 0 )
            {
                // 发送数据等一会再发
                iTimeOut -= MB_TCP_READ_CYCLE;
                DJY_EventDelay( MB_TCP_READ_CYCLE );
            }
            else
            {
                bAbort = TRUE;  //一直发不出去，不发了
            }
            break;
        case 0:
            prvvMBPortReleaseClient(  );    //关闭客户端
            bAbort = TRUE;
            break;
        default:
            iBytesSent += res;
            break;
        }
    }
    while( ( iBytesSent != usTCPLength ) && !bAbort );
    // 发送出去的数据等于要发送的数据，才返回成功
    bFrameSent = iBytesSent == usTCPLength ? TRUE : FALSE;

    return bFrameSent;
}

void prvvMBPortReleaseClient(  )
{
    // 读光sock中的数据
    ( void )recv( xClientSocket, &aucTCPBuf[0], MB_TCP_BUF_SIZE, 0 );

    ( void )closesocket( xClientSocket ); //关闭sock
    xClientSocket = INVALID_SOCKET;
}

BOOL prvbMBPortAcceptClient(  )
{
    s32          xNewSocket;
    BOOL            bOkay;

    /* Check if we can handle a new connection. */
    // 检查我们是否可以处理一个新的连接
    if( xClientSocket != INVALID_SOCKET )
    {
        error_printf( "modbus tcp", "can't accept new client. all connections in use.\n" );
        bOkay = FALSE;
    }
    else if( ( xNewSocket = accept( xListenSocket, NULL, NULL ) ) == INVALID_SOCKET )
    {
        bOkay = FALSE;
    }
    else
    {
        if (Multiplex_AddObject(ModbusTCP_ClientSet, xNewSocket, CN_SOCKET_IOREAD |  CN_MULTIPLEX_SENSINGBIT_ET, 0))
        {
            xClientSocket = xNewSocket;
            usTCPBufPos = 0;
            usTCPFrameBytesLeft = MB_TCP_FUNC;
            bOkay = TRUE;
        }
        else
        {
            closesocket(xNewSocket);
            error_printf( "modbus tcp", "add xNewSocket to ModbusTCP_ClientSet fail!\n\r");
            return FALSE;
        }
    }
    return bOkay;
}
