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


#include <stdio.h>
#include <string.h>
#include <net/sys/socket.h>
#include <net/netdb.h>
#include "py/objtuple.h"
#include "py/objlist.h"
#include "py/runtime.h"
#include "py/stream.h"
#include "py/mperrno.h"
#include "lib/netutils/netutils.h"

//todo: all function ,I haven't done error handling yet

#if MICROPY_PY_DJYIP

#define MOD_NETWORK_IPADDR_BUF_SIZE (4)

#define MOD_NETWORK_AF_INET (2)
#define MOD_NETWORK_AF_INET6 (10)

#define MOD_NETWORK_SOCK_STREAM (1)
#define MOD_NETWORK_SOCK_DGRAM (2)
#define MOD_NETWORK_SOCK_RAW (3)

typedef struct _mod_network_socket_obj_t
{
    mp_obj_base_t base;
    s32 mysockfd;
} mod_network_socket_obj_t;

/******************************************************************************/
// socket class

STATIC const mp_obj_type_t socket_type;

// constructor socket(family=AF_INET, type=SOCK_STREAM, proto=0, fileno=None)
STATIC mp_obj_t socket_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args)
{
    s32 family = AF_INET;           //set to default
    s32 mytype = SOCK_STREAM;         //set to default
    s32 protocol = IPPROTO_TCP;     //set to default
    (void)type; (void)n_kw;
    if (n_args >= 1)
        family = mp_obj_get_int(args[0]);
    if (n_args >= 2)
        mytype = mp_obj_get_int(args[1]);
    if (n_args >= 3)
        protocol = mp_obj_get_int(args[2]);

    // create socket object (not bound to any NIC yet)
    mod_network_socket_obj_t *s = m_new_obj_with_finaliser(mod_network_socket_obj_t);
    s->base.type = &socket_type;
    s->mysockfd = socket(family, mytype, protocol);

    return MP_OBJ_FROM_PTR(s);
}

// method socket.bind(address)
STATIC mp_obj_t socket_bind(mp_obj_t self_in, mp_obj_t addr_in)
{
    mod_network_socket_obj_t *self = MP_OBJ_TO_PTR(self_in);
    struct sockaddr_in ipportaddr;

    // get address
    ipportaddr.sin_port = netutils_parse_inet_addr(addr_in, (u8*)&ipportaddr.sin_addr, NETUTILS_BIG);
    ipportaddr.sin_family = AF_INET;
    bind(self->mysockfd, (struct sockaddr *)&ipportaddr, sizeof(ipportaddr));

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_bind_obj, socket_bind);

// method socket.listen(backlog)
STATIC mp_obj_t socket_listen(mp_obj_t self_in, mp_obj_t backlog)
{
    mod_network_socket_obj_t *self = MP_OBJ_TO_PTR(self_in);

    listen(self->mysockfd, mp_obj_get_int(backlog));
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_listen_obj, socket_listen);

// method socket.accept()
STATIC mp_obj_t socket_accept(mp_obj_t self_in)
{
    mod_network_socket_obj_t *self = MP_OBJ_TO_PTR(self_in);
    s32 addrlen;
    struct sockaddr_in ipportaddr;

    // create new socket object
    mod_network_socket_obj_t *socket2 = m_new_obj_with_finaliser(mod_network_socket_obj_t);

    // get address
    socket2->mysockfd = accept(self->mysockfd, (struct sockaddr *)&ipportaddr, &addrlen);

    // make the return value
    mp_obj_tuple_t *client = MP_OBJ_TO_PTR(mp_obj_new_tuple(2, NULL));
    client->items[0] = MP_OBJ_FROM_PTR(socket2);
    client->items[1] = netutils_format_inet_addr((u8*)&ipportaddr.sin_addr,
                                            ipportaddr.sin_port, NETUTILS_BIG);

    return MP_OBJ_FROM_PTR(client);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(socket_accept_obj, socket_accept);

// method socket.connect(address)
STATIC mp_obj_t socket_connect(mp_obj_t self_in, mp_obj_t addr_in)
{
    mod_network_socket_obj_t *self = MP_OBJ_TO_PTR(self_in);
    struct sockaddr_in ipportaddr;

    // get address
    ipportaddr.sin_port = netutils_parse_inet_addr(addr_in, (u8*)&ipportaddr.sin_addr, NETUTILS_BIG);
    ipportaddr.sin_family = AF_INET;
    connect(self->mysockfd, (struct sockaddr *)&ipportaddr, sizeof(ipportaddr));

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_connect_obj, socket_connect);

// method socket.send(bytes)
STATIC mp_obj_t socket_send(mp_obj_t self_in, mp_obj_t buf_in)
{
    mod_network_socket_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf_in, &bufinfo, MP_BUFFER_READ);
    mp_uint_t ret = send(self->mysockfd, bufinfo.buf, bufinfo.len, 0);
    return mp_obj_new_int_from_uint(ret);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_send_obj, socket_send);

// method socket.recv(bufsize)
STATIC mp_obj_t socket_recv(mp_obj_t self_in, mp_obj_t len_in)
{
    mod_network_socket_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_int_t len = mp_obj_get_int(len_in);
    vstr_t vstr;
    mp_int_t ret;

    vstr_init_len(&vstr, len);
    ret = recv(self->mysockfd, (byte *)vstr.buf,len, 0);

    if (ret == 0) {
        return mp_const_empty_bytes;
    }
    else
    {
        vstr.len = ret;
        return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_recv_obj, socket_recv);

// method socket.sendto(bytes, address)
STATIC mp_obj_t socket_sendto(mp_obj_t self_in, mp_obj_t data_in, mp_obj_t addr_in)
{
    mod_network_socket_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_int_t ret;
    struct sockaddr_in ipportaddr;

    // get the data
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(data_in, &bufinfo, MP_BUFFER_READ);


    // get address
    ipportaddr.sin_port = netutils_parse_inet_addr(addr_in, (u8*)&ipportaddr.sin_addr, NETUTILS_BIG);
    ipportaddr.sin_family = AF_INET;

    ret = sendto(self->mysockfd, bufinfo.buf, bufinfo.len, 0, (struct sockaddr *)&ipportaddr, sizeof(ipportaddr));

    return mp_obj_new_int(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(socket_sendto_obj, socket_sendto);

// method socket.recvfrom(bufsize)
STATIC mp_obj_t socket_recvfrom(mp_obj_t self_in, mp_obj_t len_in) {
    mod_network_socket_obj_t *self = MP_OBJ_TO_PTR(self_in);
    struct sockaddr_in ipportaddr;
    mp_int_t ret;
    vstr_t vstr;
    s32 len =  sizeof(ipportaddr);

    vstr_init_len(&vstr, mp_obj_get_int(len_in));

    ret = recvfrom(self->mysockfd, vstr.buf, vstr.len,0,(struct sockaddr *)&ipportaddr,&len);

    mp_obj_t tuple[2];
    if (ret == 0) {
        tuple[0] = mp_const_empty_bytes;
    } else {
        vstr.len = ret;
        tuple[0] = mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
    }
    tuple[1] = netutils_format_inet_addr((u8*)&ipportaddr.sin_addr, ipportaddr.sin_port, NETUTILS_BIG);
    return mp_obj_new_tuple(2, tuple);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_recvfrom_obj, socket_recvfrom);

// method socket.setsockopt(level, optname, value)
STATIC mp_obj_t socket_setsockopt(size_t n_args, const mp_obj_t *args)
{
    mod_network_socket_obj_t *self = MP_OBJ_TO_PTR(args[0]);

    mp_int_t level = mp_obj_get_int(args[1]);
    mp_int_t opt = mp_obj_get_int(args[2]);

    (void)n_args;
    const void *optval;
    mp_uint_t optlen;
    mp_int_t val;
    if (mp_obj_is_integer(args[3])) {
        val = mp_obj_get_int_truncated(args[3]);
        optval = &val;
        optlen = sizeof(val);
    } else
    {
        mp_buffer_info_t bufinfo;
        mp_get_buffer_raise(args[3], &bufinfo, MP_BUFFER_READ);
        optval = bufinfo.buf;
        optlen = bufinfo.len;
    }

    if (setsockopt(self->mysockfd, level, opt, optval, optlen) != 0)
    {
        mp_raise_OSError(errno);
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(socket_setsockopt_obj, 4, 4, socket_setsockopt);

// method socket.settimeout(value)
// timeout=0 means non-blocking
// timeout=None means blocking
// otherwise, timeout is in seconds
STATIC mp_obj_t socket_settimeout(mp_obj_t self_in, mp_obj_t timeout_in) {
    mod_network_socket_obj_t *self = MP_OBJ_TO_PTR(self_in);

    mp_uint_t timeout;
    if (timeout_in == mp_const_none) {
        timeout = -1;
    } else {
        #if MICROPY_PY_BUILTINS_FLOAT
        timeout = (mp_uint_t)(MICROPY_FLOAT_CONST(1000.0) * mp_obj_get_float(timeout_in));
        #else
        timeout = 1000 * mp_obj_get_int(timeout_in);
        #endif
    }
    setsockopt(self->mysockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    setsockopt(self->mysockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_settimeout_obj, socket_settimeout);

// method socket.setblocking(flag)
STATIC mp_obj_t socket_setblocking(mp_obj_t self_in, mp_obj_t blocking) {
    if (mp_obj_is_true(blocking)) {
        return socket_settimeout(self_in, mp_const_none);
    } else {
        return socket_settimeout(self_in, MP_OBJ_NEW_SMALL_INT(0));
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_setblocking_obj, socket_setblocking);

STATIC const mp_rom_map_elem_t socket_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&mp_stream_close_obj) },
    { MP_ROM_QSTR(MP_QSTR_close), MP_ROM_PTR(&mp_stream_close_obj) },
    { MP_ROM_QSTR(MP_QSTR_bind), MP_ROM_PTR(&socket_bind_obj) },
    { MP_ROM_QSTR(MP_QSTR_listen), MP_ROM_PTR(&socket_listen_obj) },
    { MP_ROM_QSTR(MP_QSTR_accept), MP_ROM_PTR(&socket_accept_obj) },
    { MP_ROM_QSTR(MP_QSTR_connect), MP_ROM_PTR(&socket_connect_obj) },
    { MP_ROM_QSTR(MP_QSTR_send), MP_ROM_PTR(&socket_send_obj) },
    { MP_ROM_QSTR(MP_QSTR_recv), MP_ROM_PTR(&socket_recv_obj) },
    { MP_ROM_QSTR(MP_QSTR_sendto), MP_ROM_PTR(&socket_sendto_obj) },
    { MP_ROM_QSTR(MP_QSTR_recvfrom), MP_ROM_PTR(&socket_recvfrom_obj) },
    { MP_ROM_QSTR(MP_QSTR_setsockopt), MP_ROM_PTR(&socket_setsockopt_obj) },
    { MP_ROM_QSTR(MP_QSTR_settimeout), MP_ROM_PTR(&socket_settimeout_obj) },
    { MP_ROM_QSTR(MP_QSTR_setblocking), MP_ROM_PTR(&socket_setblocking_obj) },
};

STATIC MP_DEFINE_CONST_DICT(socket_locals_dict, socket_locals_dict_table);

mp_uint_t socket_ioctl(mp_obj_t self_in, mp_uint_t request, uintptr_t arg, int *errcode) {
    mod_network_socket_obj_t *self = MP_OBJ_TO_PTR(self_in);
    (void)arg; (void)errcode;
    if (request == MP_STREAM_CLOSE) {
        return closesocket(self->mysockfd);
    }
    else
        return 0;
}

STATIC const mp_stream_p_t socket_stream_p = {
    .ioctl = socket_ioctl,
    .is_text = false,
};

STATIC const mp_obj_type_t socket_type = {
    { &mp_type_type },
    .name = MP_QSTR_socket,
    .make_new = socket_make_new,
    .protocol = &socket_stream_p,
    .locals_dict = (mp_obj_dict_t *)&socket_locals_dict,
};

/******************************************************************************/
// usocket module

// function usocket.getaddrinfo(host, port)
STATIC mp_obj_t mod_usocket_getaddrinfo(mp_obj_t host_in, mp_obj_t port_in)
{
    size_t hlen;
    struct hostent *myhostent;
    const char *host = mp_obj_str_get_data(host_in, &hlen);
    mp_int_t port = mp_obj_get_int(port_in);
//  uint8_t out_ip[MOD_NETWORK_IPADDR_BUF_SIZE];
    bool have_ip = false;

    if (hlen > 0) {
        // check if host is already in IP form
        nlr_buf_t nlr;
        if (nlr_push(&nlr) == 0) {
            netutils_parse_ipv4_addr(host_in, (u8*)myhostent->h_addr_list[0], NETUTILS_BIG);
            have_ip = true;
            nlr_pop();
        } else {
            // swallow exception: host was not in IP form so need to do DNS lookup
        }
    }

    if (!have_ip)
    {
        myhostent = gethostbyname(host);
        if (myhostent != NULL)
        {
            have_ip = true;
        }
    }

    if (!have_ip) {
        mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("no available NIC"));
    }

    mp_obj_tuple_t *tuple = MP_OBJ_TO_PTR(mp_obj_new_tuple(5, NULL));
    tuple->items[0] = MP_OBJ_NEW_SMALL_INT(MOD_NETWORK_AF_INET);
    tuple->items[1] = MP_OBJ_NEW_SMALL_INT(MOD_NETWORK_SOCK_STREAM);
    tuple->items[2] = MP_OBJ_NEW_SMALL_INT(0);
    tuple->items[3] = MP_OBJ_NEW_QSTR(MP_QSTR_);
    tuple->items[4] = netutils_format_inet_addr(myhostent->h_addr_list[0], port, NETUTILS_BIG);
    return mp_obj_new_list(1, (mp_obj_t *)&tuple);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_usocket_getaddrinfo_obj, mod_usocket_getaddrinfo);

STATIC const mp_rom_map_elem_t mp_module_usocket_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_usocket) },

    { MP_ROM_QSTR(MP_QSTR_socket), MP_ROM_PTR(&socket_type) },
    { MP_ROM_QSTR(MP_QSTR_getaddrinfo), MP_ROM_PTR(&mod_usocket_getaddrinfo_obj) },

    // class constants
    { MP_ROM_QSTR(MP_QSTR_AF_INET), MP_ROM_INT(MOD_NETWORK_AF_INET) },
    { MP_ROM_QSTR(MP_QSTR_AF_INET6), MP_ROM_INT(MOD_NETWORK_AF_INET6) },

    { MP_ROM_QSTR(MP_QSTR_SOCK_STREAM), MP_ROM_INT(MOD_NETWORK_SOCK_STREAM) },
    { MP_ROM_QSTR(MP_QSTR_SOCK_DGRAM), MP_ROM_INT(MOD_NETWORK_SOCK_DGRAM) },
    { MP_ROM_QSTR(MP_QSTR_SOCK_RAW), MP_ROM_INT(MOD_NETWORK_SOCK_RAW) },

    /*
    { MP_ROM_QSTR(MP_QSTR_IPPROTO_IP), MP_ROM_INT(MOD_NETWORK_IPPROTO_IP) },
    { MP_ROM_QSTR(MP_QSTR_IPPROTO_ICMP), MP_ROM_INT(MOD_NETWORK_IPPROTO_ICMP) },
    { MP_ROM_QSTR(MP_QSTR_IPPROTO_IPV4), MP_ROM_INT(MOD_NETWORK_IPPROTO_IPV4) },
    { MP_ROM_QSTR(MP_QSTR_IPPROTO_TCP), MP_ROM_INT(MOD_NETWORK_IPPROTO_TCP) },
    { MP_ROM_QSTR(MP_QSTR_IPPROTO_UDP), MP_ROM_INT(MOD_NETWORK_IPPROTO_UDP) },
    { MP_ROM_QSTR(MP_QSTR_IPPROTO_IPV6), MP_ROM_INT(MOD_NETWORK_IPPROTO_IPV6) },
    { MP_ROM_QSTR(MP_QSTR_IPPROTO_RAW), MP_ROM_INT(MOD_NETWORK_IPPROTO_RAW) },
    */
};

STATIC MP_DEFINE_CONST_DICT(mp_module_usocket_globals, mp_module_usocket_globals_table);

const mp_obj_module_t mp_module_usocket = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&mp_module_usocket_globals,
};

#endif // MICROPY_PY_DJYIP
